import os
import csv
from optparse import OptionParser


def read_list(list_path, to_data):
    if os.path.isfile(list_path):
        with open(list_path, 'r') as f:
            for row in csv.reader(f):
                to_data[row[0]] = row[1]


if __name__ == '__main__':
    p = OptionParser()
    p.add_option("--root1", action="store",
                 dest="root1", type="string")
    p.add_option("--csv1", action="store",
                 dest="csv1", type="string")
    p.add_option("--root1_has", action="store",
                 dest="root1_has", type="string")
    p.add_option("--root2", action="store",
                 dest="root2", type="string")
    p.add_option("--csv2", action="store",
                 dest="csv2", type="string")
    p.add_option("--root2_has", action="store",
                 dest="root2_has", type="string")
    p.add_option("--common_has", action="store",
                 dest="common_has", type="string")
    p.add_option("--common_but_diff", action="store",
                 dest="common_but_diff", type="string")
    (opts, args) = p.parse_args()
    list1, list2 = {}, {}
    read_list(opts.csv1, list1)
    read_list(opts.csv2, list2)
    list1_has, list2_has, common_has, common_but_diff = [], [], [], []
    for i in list1:
        if not list2.get(i):
            list1_has.append(i)
        else:
            common_has.append(i)

    for i in list2:
        if not list1.get(i):
            list2_has.append(i)

    for i in common_has:
        if list1.get(i) != list2.get(i):
            common_but_diff.append(i)

    with open(opts.root1_has, 'w', newline='', encoding='utf-8') as f:
        for v in list1_has:
            f.write(v)
            f.write("\n")
    with open(opts.root2_has, 'w', newline='', encoding='utf-8') as f:
        for v in list2_has:
            f.write(v)
            f.write("\n")
    with open(opts.common_has, 'w', newline='', encoding='utf-8') as f:
        for v in common_has:
            f.write(v)
            f.write("\n")
    with open(opts.common_but_diff, 'w', newline='', encoding='utf-8') as f:
        for v in common_but_diff:
            f.write(v)
            f.write("\n")
    print("分析目录不同已经完成")
