from optparse import OptionParser


parser = OptionParser()
parser.add_option("--root", action="store",
                  dest="root", type="string")
parser.add_option("--txt", action="store",
                  dest="txt", type="string")
(opts, args) = parser.parse_args()


def convert_gb2312_to_utf8(src_path, dst_path):
    with open(src_path, 'r', encoding='gb2312', errors='ignore') as f_in:
        content = f_in.read()

    with open(dst_path, 'w', encoding='utf-8') as f_out:
        f_out.write(content)

    print(f"Converted {src_path} → {dst_path}")


try:
    with open(opts.txt, 'r', encoding='utf-8') as f:
        need_deal = [line.strip()
                     for line in f if line.strip()]  # 去掉空行和换行
except ValueError:
    with open(opts.txt, 'r', encoding='gb2312') as f:
        need_deal = [line.strip()
                     for line in f if line.strip()]  # 去掉空行和换行


# 示例用法
for i in need_deal:
    convert_gb2312_to_utf8(opts.root + i, opts.root + i)
