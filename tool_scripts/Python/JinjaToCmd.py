import configparser
from optparse import OptionParser

try:
    from jinja2 import Template
except:
    import subprocess
    import sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "Jinja2"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    from jinja2 import Template
p = OptionParser()
p.add_option("--config", action="store",dest="config", type="string")
p.add_option("--j2", action="store",dest="j2", type="string")
p.add_option("--cmd", action="store",dest="cmd", type="string")
p.add_option("--config_file", action="store",dest="config_file", type="string")
(opts, args) = p.parse_args()


# 用 configparser 读取 ini 配置
def read_config(filename):
    parser = configparser.ConfigParser()
    parser.read(filename, encoding='utf-8')
    return dict(parser[opts.config])

# 读取模板文件
with open(opts.j2, 'r', encoding='utf-8') as f:
    template_content = f.read()

# 渲染模板
context = read_config(opts.config_file)
template = Template(template_content)
output = template.render(**context)

# 写入最终文件
with open(opts.cmd, 'w', encoding='utf-8') as f:
    f.write(output)

print("✔ 成功生成 xxx.cmd")