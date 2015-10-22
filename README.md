# readslk
read .slk file

http://sstask.com/2015/09/25/slkread/

格式
>**第一行 ID;PWXL;N;E**
>**最后一行 E**
>**表示大小的行 B;Y4;X2;D0** *即总共4行2列*
>**内容行 C;X2;N;K33** *第2列内容33，行数向上找最近的Y;K后面的内容以\r\n结束或者遇“”之外的;结束*

解析
>**字符串内容在最大匹配的两个""之间**
>**字符串中1b 20 3a三个连续字符表示换行，解析时需用\n替换**
>**字符串中两个连续的;表示一个; 即;;要解析成;**

格式参考https://en.wikipedia.org/wiki/SYmbolic_LinK_%28SYLK%29
