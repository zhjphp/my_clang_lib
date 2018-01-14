#include <stdio.h>

unsigned long copy_file(char *, char *);

int main(int argc, char *argv[])
{
	char *src;
	char *dest;
	unsigned long res;

	src = argv[1];
	dest = argv[2];

	printf("src = %s \n", src);
	printf("dest = %s \n", dest);

	res = copy_file(src, dest);

	printf("return = %d \n", res);
}

unsigned long copy_file(char *src, char *dest)
{
	//读写文件指针
	FILE *pfRead;
	FILE *pfWrite;

	//打开文件
	pfRead = fopen(src, "rb");
	pfWrite = fopen(dest, "wb");

	//打开文件是否成功
	if ((pfRead == NULL) || (pfWrite == NULL))
	{
		fclose(pfRead);
		fclose(pfWrite);
		printf("打开文件错误");
		return -1;
	}

	unsigned long copy_size = 0;

	int buf_size = 512;						 //设置缓冲区大小
	char buffer[buf_size];					 //缓冲区起始地址指针
	size_t element_size = sizeof(buffer[0]); //单个元素大小
	int copy_len;							 //单次复制内容大小

	//执行复制
	do
	{
		copy_len = fread(buffer, element_size, buf_size, pfRead);
		fwrite(buffer, element_size, buf_size, pfWrite);

		copy_size += copy_len * element_size;
	} while (copy_len == buf_size);

	//复制完成关闭文件
	fclose(pfRead);
	fclose(pfWrite);

	return copy_size;
}
