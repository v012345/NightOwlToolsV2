import subprocess
import sys
try: import pandas
except: subprocess.Popen([sys.executable, "-m", "pip", "install", "pandas"], creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
try: import openpyxl
except: subprocess.Popen([sys.executable, "-m", "pip", "install", "openpyxl"], creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
