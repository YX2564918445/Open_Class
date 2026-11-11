"""把 51_Course 下的 markdown 讲义编译成 PDF，样式对齐 pdf/01~06 现有六份。

做法:
  1. md-to-pdf(Puppeteer) 渲染 -> 保留目录内链跳转
  2. PyMuPDF 在每页固定位置盖 logo（现有六份 PDF 的 logo 就是逐页同一位置）
  3. 校验字号/颜色/页边距

用法:
    python build_pdf.py 07_矩阵按键.md 08_外部中断0.md 09_外部中断1.md
    python build_pdf.py --all
输出到 ./pdf/
"""
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

BASE = Path(__file__).resolve().parent
CSS = BASE / "pdf-style.css"
OUT = Path(os.environ.get("PDF_OUT") or (BASE / "pdf"))
if not OUT.is_absolute():
    OUT = BASE / OUT
LOGO = BASE / "assets" / "logo.png"

# 现有六份 PDF 里 logo 的精确位置（pt）
LOGO_RECT = (243.6, 25.5, 351.3, 48.5)

PDF_OPTIONS = {
    "format": "A4",
    "printBackground": True,
    "preferCSSPageSize": True,
}
LAUNCH_OPTIONS = {
    "executablePath": r"C:\Program Files\Google\Chrome\Application\chrome.exe",
    "args": ["--no-sandbox", "--disable-gpu"],
}


def stamp_logo(pdf_path: Path) -> int:
    """在每页盖 logo，返回处理页数。"""
    import pymupdf

    doc = pymupdf.open(str(pdf_path))
    rect = pymupdf.Rect(*LOGO_RECT)
    for page in doc:
        page.insert_image(rect, filename=str(LOGO), keep_proportion=False, overlay=True)
    tmp = pdf_path.with_suffix(".stamped.pdf")
    doc.save(str(tmp), garbage=3, deflate=True)
    doc.close()
    tmp.replace(pdf_path)
    return len(pymupdf.open(str(pdf_path)))


def build(md_path: Path) -> Path:
    """编译单个 markdown，返回输出 PDF 路径。"""
    tmp_md = md_path.with_name(f"_build_{md_path.name}")
    tmp_md.write_text(md_path.read_text(encoding="utf-8"), encoding="utf-8")

    cmd = [
        "npx", "md-to-pdf", tmp_md.name,
        "--stylesheet", CSS.name,
        "--pdf-options", json.dumps(PDF_OPTIONS),
        "--launch-options", json.dumps(LAUNCH_OPTIONS),
        "--highlight-style", "github",
    ]
    proc = subprocess.run(
        cmd, cwd=str(BASE), capture_output=True, text=True,
        encoding="utf-8", errors="replace", shell=True, timeout=240,
    )
    tmp_pdf = tmp_md.with_suffix(".pdf")
    dst = OUT / (md_path.stem + ".pdf")

    try:
        if not tmp_pdf.exists():
            tail = (proc.stdout or "") + (proc.stderr or "")
            raise RuntimeError(f"生成失败: {md_path.name}\n{tail[-900:]}")
        OUT.mkdir(exist_ok=True)
        shutil.move(str(tmp_pdf), dst)
        pages = stamp_logo(dst)
        print(f"  OK  {dst.name}  {pages} 页  {dst.stat().st_size/1024:.0f} KB")
        return dst
    finally:
        tmp_md.unlink(missing_ok=True)


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    if argv[1] == "--all":
        targets = sorted(BASE.glob("*.md"))
    else:
        targets = [BASE / a for a in argv[1:]]
    for md in targets:
        if not md.exists():
            print(f"  跳过（不存在）: {md.name}")
            continue
        print(f"编译 {md.name}")
        build(md)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))