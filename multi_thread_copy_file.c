#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 512
#define PTHREAD_NUMBER 4

typedef struct copy_block
{
	char fin[BUFF_SIZE];
	char fout[BUFF_SIZE];
	long start;		   //起始位置
	long segment_size; //分段大小
	int id;			   //虚拟线程id
} __attribute__((packed)) page;

long file_size(char *filename)
{
	struct stat fstat;
	memset(&fstat, 0, sizeof(fstat));
	stat(filename, &fstat);
	return fstat.st_size;
}

//单线程任务逻辑
void pthread_copy(void *arg)
{
	//转换指针类型
	page *p = (page *)arg;
	//printf("src = %s \n ", p->fin);
	//printf("desc = %s \n ", p->fout);
	//printf("id = %d \n ", p->id);

	//打开文件
	FILE *fin = fopen(p->fin, "r");
	FILE *fout = fopen(p->fout, "rb+");

	//printf("p[%d]  start %d \n", p->id, p->start);
	// printf("p[%d]  size %d \n", p->id, p->segment_size);

	// int dq1 = ftell(fin);
	// int dq2 = ftell(fout);

	// printf("---p[%d] dq1 %d \n", p->id, dq1);
	// printf("---p[%d] dq2 %d \n", p->id, dq2);

	//移动流到偏移位置
	if (p->start > 0) {
		int res1 = fseek(fin, p->start, SEEK_SET);
		int res2 = fseek(fout, p->start, SEEK_SET);
	}

	long site = ftell(fout);
	printf("p[%d]  start %d \n", p->id, site);

	// dq1 = ftell(fin);
	// dq2 = ftell(fout);

	// printf("p[%d] dq1 %d \n", p->id, dq1);
	// printf("p[%d] dq2 %d \n", p->id, dq2);

	// printf("res1 %d \n", res1);
	// printf("res2 %d \n", res2);

	//开始复制
	char buffer[BUFF_SIZE];		 //读写区
	long read_size = BUFF_SIZE;  //预设读写大小
	long left = p->segment_size; //剩余大小，初始化为任务总大小
	long reade_len = 0;			 //读取文件大小
	long total_len = 0;

	//printf("---p[%d] left = %d \n ", p->id, left);

	//当剩余大小大于0时保持复制
	while (left > 0)
	{
		//如果文件剩余大小小于预设读写大小，则按剩余大小读取
		if (read_size > left)
		{
			read_size = left;
		}
		//读取文件
		reade_len = fread(buffer, 1, read_size, fin);
		total_len += reade_len;
		//写入文件
		if (reade_len > 0)
		{
			fwrite(buffer, 1, reade_len, fout);
		}

		//剩余大小减去已读写大小
		left = left - reade_len;
	}

	//printf("p[%d] left = %d \n ", p->id, left);

	//printf("p[%d] copy size = %d \n ", p->id, copy_size);
	site = ftell(fout);
	printf("p[%d]  end %d \n", p->id, site);

	printf("p[%d]  all %d \n", p->id, total_len);

	//复制完成关闭文件
	fclose(fin);
	int x = fclose(fout);
	printf("p[%d] x =  %d \n", p->id, x);
	pthread_exit(NULL);
	//printf("p[%d] 复制大小为 %d \n", p->id, copy_len);
}

//开启多线程任务
int multi_copy(char *src, char *dest)
{
	//判断文件是否存在,以及是否具有读取权限
	int file_exist = access(src, 4);
	if (file_exist != 0)
		fprintf(stderr, "源文件不存在");

	//获取文件大小
	long fsize = file_size(src);

	//创建目标文件
	FILE *fc = fopen(dest, "w");
	if(fc == NULL){
		fprintf(stderr, "创建文件失败");
	}

	//真正运行线程数量
	int real_pthread_number = PTHREAD_NUMBER;
	if (fsize < PTHREAD_NUMBER)
		real_pthread_number = 1;

	//给任务结构体分配内存
	page *p;
	p = malloc(sizeof(*p) * PTHREAD_NUMBER);

	long offset = 0;									  //文本偏移量
	long segment = fsize / real_pthread_number;			  //分段长度
	long segment_remainder = fsize % real_pthread_number; //分段后剩余长度

	//给每个线程分配任务
	for (int i = 0; i < real_pthread_number; i++)
	{
		//分配复制任务的文件大小
		if (i + 1 == real_pthread_number)
		{
			p[i].segment_size = segment + segment_remainder;
		}
		else
		{
			p[i].segment_size = segment;
		}

		//printf("p[%d] segment_size %d \n", i, p[i].segment_size);

		//确定任务的起止位置
		p[i].start = offset;
		offset = offset + p[i].segment_size;

		//printf("offsite = %d \n", offset);

		//文件路径存入任务
		strncpy(p[i].fin, src, strlen(src));
		//printf("src = %s \n", p[i].fin);
		strncpy(p[i].fout, dest, strlen(dest));
		//printf("dest = %s \n", p[i].fout);

		//分配虚拟线程id
		p[i].id = i;
	}

	//创建线程
	pthread_t work[real_pthread_number];
	for (int i = 0; i < real_pthread_number; i++)
	{
		pthread_create(&work[i], NULL, (void *)&pthread_copy, (void *)&p[i]);
	}

	//阻塞主线程
	for (int i = 0; i < real_pthread_number; i++)
	{
		pthread_join(work[i], NULL);
	}

	//释放任务结构体占用内存
	if (p != NULL)
	{
		free(p);
		p = NULL;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char *src;
	char *dest;

	src = argv[1];
	dest = argv[2];

	multi_copy(src, dest);
}
