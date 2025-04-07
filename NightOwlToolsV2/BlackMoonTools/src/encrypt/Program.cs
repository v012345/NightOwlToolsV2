using System.IO;
using System.Text;

namespace decrpt
{
	internal class Program
	{
		private static void convertFile(string path)
		{
			byte[] array = File.ReadAllBytes(path);
			int num = array.Length;
			byte[] bytes = Encoding.Default.GetBytes("AONESOFT");
			int num2 = bytes.Length;
			int i = 0;
			int num3 = 0;
			int num4 = 0;
			for (; i < num; i += num / 10)
			{
				while (i < num && num4 < 64)
				{
					array[i] = (byte)(array[i] ^ bytes[num3]);
					i++;
					num3 = (num3 + 1) % num2;
					num4++;
				}
				num4 = 0;
			}
			byte[] array2 = new byte[4] { 14, 11, 28, 18 };
			FileStream fileStream = new FileStream(path, FileMode.Create, FileAccess.ReadWrite);
			fileStream.Write(array2, 0, array2.Length);
			fileStream.Write(array, 0, array.Length);
			fileStream.Close();
		}

		private static void converDirectory(FileSystemInfo info)
		{
			if (!info.Exists)
			{
				return;
			}
			DirectoryInfo directoryInfo = info as DirectoryInfo;
			if (directoryInfo == null)
			{
				return;
			}
			FileSystemInfo[] fileSystemInfos = directoryInfo.GetFileSystemInfos();
			for (int i = 0; i < fileSystemInfos.Length; i++)
			{
				FileInfo fileInfo = fileSystemInfos[i] as FileInfo;
				if (fileInfo != null)
				{
					convertFile(fileInfo.FullName);
				}
				else
				{
					converDirectory(fileSystemInfos[i]);
				}
			}
		}

		private static void Main(string[] args)
		{
			foreach (string path in args)
			{
				if (File.Exists(path))
				{
					convertFile(path);
				}
				else if (Directory.Exists(path))
				{
					converDirectory(new DirectoryInfo(path));
				}
			}
		}
	}
}
