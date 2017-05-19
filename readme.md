# 历史版本
*****************************************************************************************
* **版　　本：V0.1.0**
* **开发目的** ：此软件为了解决答题器项目中下载程序的问题。
* **特性描述** : 
	* 使用串口直接传输HEX文件，实现代码更新
*****************************************************************************************
* **时间**：2017-04-05
* **描述** :
> 1.创建初始版本，建立一个空白的工程。

* **时间**：2017-04-07
* **描述** :
> 1.使用Cube库，创建新的工程。

* **时间**：2017-04-07
* **描述** :
> 1.完成串口部分的移植。

* **时间**：2017-04-21
* **描述** :
> 1.完成完整的程序移植。
> 2.保留掉电保存的数据，下载程序时不允许擦除。
> 3.增加是否已经有程序的判断逻辑。
> 4.增加指示灯指示下载状态。

* **时间**：2017-05-03
* **描述** :
> 1.修复工程不能调试的BUG

* **时间**：2017-05-04
* **描述** :
> 1.修复输出信息换行符的输出风格将“\n\r”改为“\r\n”

* **时间**：2017-05-05
* **描述** :
> 1.修改文件的最大长度，最大支持100个字符

*****************************************************************************************
* **版　　本：V0.1.1**
* **开发目的** ：为了解决下发程序过程中国可能出现的程序跟新不成功最后卡死的情况
* **特性描述** : 
	* 使用双备份机制，只有等待程序完全接受成功后才会擦掉原来的程序
*****************************************************************************************
* **时间**：2017-05-19
* **描述** :
> 1.创建新的版本，开始新的功能的开发


