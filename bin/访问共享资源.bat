cls

@echo off

rem 访问共享资源批处理文件
 
net use y: \\21.114.11.101\d$ "2006gsycl" /user:administrator
net use z: \\21.114.11.9\resource "2007gsycl" /user:administrator

rem net use X: \\21.114.11.8\photo "hzxxzx2007" /user:hzxxzx
