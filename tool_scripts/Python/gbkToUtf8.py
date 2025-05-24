from optparse import OptionParser


parser = OptionParser()
parser.add_option("--root", action="store",
                  dest="root", type="string")
parser.add_option("--to", action="store",
                  dest="to", type="string")
parser.add_option("--txt", action="store",
                  dest="txt", type="string")
(opts, args) = parser.parse_args()


def convert_gbk_to_utf8(src_path, dst_path):
    with open(src_path, 'r', encoding='gbk') as f_in:
        content = f_in.read()

    with open(dst_path, 'w', encoding='utf-8') as f_out:
        f_out.write(content)


try:
    with open(opts.txt, 'r', encoding='utf-8') as f:
        need_deal = [line.strip() for line in f if line.strip()]  # 去掉空行和换行
except ValueError:
    with open(opts.txt, 'r', encoding='gbk') as f:
        need_deal = [line.strip() for line in f if line.strip()]  # 去掉空行和换行


# 示例用法
for i in need_deal:
    print(f"Converted {i}")
    convert_gbk_to_utf8(opts.root + i, opts.to + i)
