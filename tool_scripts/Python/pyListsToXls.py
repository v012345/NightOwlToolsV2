from optparse import OptionParser
import importlib
import sys
import xlwt


parser = OptionParser()
parser.add_option("--mod_dir", action="store",
                  dest="mod_dir", type="string")
parser.add_option("--mod", action="store",
                  dest="mod", type="string")
parser.add_option("--to", action="store",
                  dest="to", type="string")
parser.add_option("--name", action="store",
                  dest="name", type="string")


if __name__ == '__main__':
    (opts, args) = parser.parse_args()
    sys.path.append(opts.mod_dir)
    module = importlib.import_module(opts.mod)
    cfg = getattr(module, opts.name)
    workbook = xlwt.Workbook()
    worksheet = workbook.add_sheet(opts.name)
    for row in range(len(cfg)):
        for col in range(len(cfg[row])):
            worksheet.write(row, col, cfg[row][col])
    workbook.save(opts.to)
