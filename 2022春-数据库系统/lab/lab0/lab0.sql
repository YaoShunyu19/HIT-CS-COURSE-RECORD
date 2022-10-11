create database COMPANY;
create table employee(ename varchar(20), essn varchar(20), address varchar(20), salary varchar(8), superssn varchar(20), dno varchar(4));
create table departmemt(dname varchar(20), dno varchar(4), mgrssn varchar(20), mgrstartdate DATE);
create table project(pname varchar(20), pno varchar(4), plocation varchar(20), dno varchar(4));
create table works_on(essn varchar(20), pno varchar(4), hours varchar(8));



delete from employee
delete from departmemt
delete from project
delete from works_on



insert into employee values('张红', 'zhanghong_essn', 'zhanghong_address', '8000', 'zhanghong_essn', 'dno1')
insert into employee values('张红1', 'zhanghong1_essn', 'zhanghong1_address', '7500', 'zhanghong_essn', 'dno1')
insert into employee values('张红2', 'zhanghong2_essn', 'zhanghong2_address', '7000', 'zhanghong_essn', 'dno1')
insert into employee values('张红3', 'zhanghong3_essn', 'zhanghong3_address', '6500', 'zhanghong_essn', 'dno1')
insert into employee values('张红4', 'zhanghong4_essn', 'zhanghong4_address', '6000', 'zhanghong_essn', 'dno1')
insert into employee values('张红5', 'zhanghong5_essn', 'zhanghong5_address', '5500', 'zhanghong_essn', 'dno1')
insert into employee values('张红6', 'zhanghong6_essn', 'zhanghong6_address', '5000', 'zhanghong_essn', 'dno1')
insert into employee values('张红7', 'zhanghong7_essn', 'zhanghong7_address', '4000', 'zhanghong_essn', 'dno1')
insert into employee values('张红8', 'zhanghong8_essn', 'zhanghong8_address', '3000', 'zhanghong_essn', 'dno1')
insert into employee values('张红9', 'zhanghong9_essn', 'zhanghong9_address', '2000', 'zhanghong_essn', 'dno1')

insert into employee values('张绿', 'zhanglv_essn', 'zhanglv_address', '8000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿1', 'zhanglv1_essn', 'zhanglv1_address', '7500', 'zhanglv_essn', 'dno2')
insert into employee values('张绿2', 'zhanglv2_essn', 'zhanglv2_address', '7000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿3', 'zhanglv3_essn', 'zhanglv3_address', '6500', 'zhanglv_essn', 'dno2')
insert into employee values('张绿4', 'zhanglv4_essn', 'zhanglv4_address', '6000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿5', 'zhanglv5_essn', 'zhanglv5_address', '5500', 'zhanglv_essn', 'dno2')
insert into employee values('张绿6', 'zhanglv6_essn', 'zhanglv6_address', '5000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿7', 'zhanglv7_essn', 'zhanglv7_address', '4000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿8', 'zhanglv8_essn', 'zhanglv8_address', '3000', 'zhanglv_essn', 'dno2')
insert into employee values('张绿9', 'zhanglv9_essn', 'zhanglv9_address', '2000', 'zhanglv_essn', 'dno2')

insert into employee values('张蓝', 'zhanglan_essn', 'zhanglan_address', '8000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝1', 'zhanglan1_essn', 'zhanglan1_address', '7500', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝2', 'zhanglan2_essn', 'zhanglan2_address', '7000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝3', 'zhanglan3_essn', 'zhanglan3_address', '6500', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝4', 'zhanglan4_essn', 'zhanglan4_address', '6000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝5', 'zhanglan5_essn', 'zhanglan5_address', '5500', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝6', 'zhanglan6_essn', 'zhanglan6_address', '5000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝7', 'zhanglan7_essn', 'zhanglan7_address', '4000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝8', 'zhanglan8_essn', 'zhanglan8_address', '3000', 'zhanglan_essn', 'dno3')
insert into employee values('张蓝9', 'zhanglan9_essn', 'zhanglan9_address', '2000', 'zhanglan_essn', 'dno3')

insert into employee values('张黄', 'zhanghuang_essn', 'zhanghuang_address', '8000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄1', 'zhanghuang1_essn', 'zhanghuang1_address', '7500', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄2', 'zhanghuang2_essn', 'zhanghuang2_address', '7000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄3', 'zhanghuang3_essn', 'zhanghuang3_address', '6500', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄4', 'zhanghuang4_essn', 'zhanghuang4_address', '6000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄5', 'zhanghuang5_essn', 'zhanghuang5_address', '5500', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄6', 'zhanghuang6_essn', 'zhanghuang6_address', '5000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄7', 'zhanghuang7_essn', 'zhanghuang7_address', '4000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄8', 'zhanghuang8_essn', 'zhanghuang8_address', '3000', 'zhanghuang_essn', 'dno4')
insert into employee values('张黄9', 'zhanghuang9_essn', 'zhanghuang9_address', '2000', 'zhanghuang_essn', 'dno4')

insert into employee values('张紫', 'zhangzi_essn', 'zhangzi_address', '8000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫1', 'zhangzi1_essn', 'zhangzi1_address', '7500', 'zhangzi_essn', 'dno5')
insert into employee values('张紫2', 'zhangzi2_essn', 'zhangzi2_address', '7000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫3', 'zhangzi3_essn', 'zhangzi3_address', '6500', 'zhangzi_essn', 'dno5')
insert into employee values('张紫4', 'zhangzi4_essn', 'zhangzi4_address', '6000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫5', 'zhangzi5_essn', 'zhangzi5_address', '5500', 'zhangzi_essn', 'dno5')
insert into employee values('张紫6', 'zhangzi6_essn', 'zhangzi6_address', '5000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫7', 'zhangzi7_essn', 'zhangzi7_address', '4000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫8', 'zhangzi8_essn', 'zhangzi8_address', '3000', 'zhangzi_essn', 'dno5')
insert into employee values('张紫9', 'zhangzi9_essn', 'zhangzi9_address', '2000', 'zhangzi_essn', 'dno5')


insert into departmemt values('ResearchDepartment', 'dno1', 'zhanghong_essn', '2001-1-1')
insert into departmemt values('2Department', 'dno2', 'zhanglv_essn', '2002-2-2')
insert into departmemt values('3Department', 'dno3', 'zhanglan_essn', '2003-3-3')
insert into departmemt values('4Department', 'dno4', 'zhanghuang_essn', '2004-4-4')
insert into departmemt values('5Department', 'dno5', 'zhangzi_essn', '2005-5-5')


insert into project values('project1_name', 'p1', 'p1_address', 'dno1')
insert into project values('project2_name', 'p2', 'p2_address', 'dno1')
insert into project values('project3_name', 'p3', 'p3_address', 'dno2')
insert into project values('project4_name', 'p4', 'p4_address', 'dno3')
insert into project values('project5_name', 'p5', 'p5_address', 'dno3')
insert into project values('project6_name', 'p6', 'p6_address', 'dno3')
insert into project values('project7_name', 'p7', 'p7_address', 'dno4')
insert into project values('project8_name', 'p8', 'p8_address', 'dno5')
insert into project values('project9_name', 'p9', 'p9_address', 'dno5')
insert into project values('project10_name', 'p10', 'p10_address', 'dno5')


insert into works_on values('zhanghong_essn', 'p1', '4')
insert into works_on values('zhanghong_essn', 'p2', '5')
insert into works_on values('zhanghong1_essn', 'p1', '2')
insert into works_on values('zhanghong1_essn', 'p2', '3')
insert into works_on values('zhanghong2_essn', 'p1', '3')
insert into works_on values('zhanghong2_essn', 'p2', '2')
insert into works_on values('zhanghong3_essn', 'p1', '1')
insert into works_on values('zhanghong3_essn', 'p2', '4')
insert into works_on values('zhanghong4_essn', 'p1', '4')
insert into works_on values('zhanghong4_essn', 'p2', '1')
insert into works_on values('zhanghong5_essn', 'p1', '4')
insert into works_on values('zhanghong5_essn', 'p2', '5')
insert into works_on values('zhanghong6_essn', 'p1', '3')
insert into works_on values('zhanghong6_essn', 'p2', '2')
insert into works_on values('zhanghong7_essn', 'p1', '2')
insert into works_on values('zhanghong7_essn', 'p2', '5')
insert into works_on values('zhanghong8_essn', 'p1', '3')
insert into works_on values('zhanghong8_essn', 'p2', '6')
insert into works_on values('zhanghong9_essn', 'p1', '2')
insert into works_on values('zhanghong9_essn', 'p2', '3')

insert into works_on values('zhanglv_essn', 'p3', '4')
insert into works_on values('zhanglv1_essn', 'p3', '2')
insert into works_on values('zhanglv2_essn', 'p3', '3')
insert into works_on values('zhanglv3_essn', 'p3', '1')
insert into works_on values('zhanglv4_essn', 'p3', '4')
insert into works_on values('zhanglv5_essn', 'p3', '4')
insert into works_on values('zhanglv6_essn', 'p3', '3')
insert into works_on values('zhanglv7_essn', 'p3', '2')
insert into works_on values('zhanglv8_essn', 'p3', '3')
insert into works_on values('zhanglv9_essn', 'p3', '2')

insert into works_on values('zhanglan_essn', 'p4', '4')
insert into works_on values('zhanglan_essn', 'p5', '5')
insert into works_on values('zhanglan_essn', 'p6', '5')
insert into works_on values('zhanglan1_essn', 'p4', '4')
insert into works_on values('zhanglan1_essn', 'p5', '5')
insert into works_on values('zhanglan1_essn', 'p6', '5')
insert into works_on values('zhanglan2_essn', 'p4', '4')
insert into works_on values('zhanglan2_essn', 'p5', '5')
insert into works_on values('zhanglan2_essn', 'p6', '5')
insert into works_on values('zhanglan3_essn', 'p4', '4')
insert into works_on values('zhanglan3_essn', 'p5', '5')
insert into works_on values('zhanglan3_essn', 'p6', '5')
insert into works_on values('zhanglan4_essn', 'p4', '4')
insert into works_on values('zhanglan4_essn', 'p5', '5')
insert into works_on values('zhanglan4_essn', 'p6', '5')
insert into works_on values('zhanglan5_essn', 'p4', '4')
insert into works_on values('zhanglan5_essn', 'p5', '5')
insert into works_on values('zhanglan5_essn', 'p6', '5')
insert into works_on values('zhanglan6_essn', 'p4', '4')
insert into works_on values('zhanglan6_essn', 'p5', '5')
insert into works_on values('zhanglan6_essn', 'p6', '5')
insert into works_on values('zhanglan7_essn', 'p4', '4')
insert into works_on values('zhanglan7_essn', 'p5', '5')
insert into works_on values('zhanglan7_essn', 'p6', '5')
insert into works_on values('zhanglan8_essn', 'p4', '4')
insert into works_on values('zhanglan8_essn', 'p5', '5')
insert into works_on values('zhanglan8_essn', 'p6', '5')
insert into works_on values('zhanglan9_essn', 'p4', '4')
insert into works_on values('zhanglan9_essn', 'p5', '5')
insert into works_on values('zhanglan9_essn', 'p6', '5')

insert into works_on values('zhanghuang_essn', 'p7', '4')
insert into works_on values('zhanghuang1_essn', 'p7', '2')
insert into works_on values('zhanghuang2_essn', 'p7', '3')
insert into works_on values('zhanghuang3_essn', 'p7', '1')
insert into works_on values('zhanghuang4_essn', 'p7', '4')
insert into works_on values('zhanghuang5_essn', 'p7', '4')
insert into works_on values('zhanghuang6_essn', 'p7', '3')
insert into works_on values('zhanghuang7_essn', 'p7', '2')
insert into works_on values('zhanghuang8_essn', 'p7', '3')
insert into works_on values('zhanghuang9_essn', 'p7', '2')

insert into works_on values('zhangzi_essn', 'p8', '4')
insert into works_on values('zhangzi_essn', 'p9', '5')
insert into works_on values('zhangzi_essn', 'p10', '2')
insert into works_on values('zhangzi1_essn', 'p8', '2')
insert into works_on values('zhangzi1_essn', 'p9', '3')
insert into works_on values('zhangzi1_essn', 'p10', '4')
insert into works_on values('zhangzi2_essn', 'p8', '3')
insert into works_on values('zhangzi2_essn', 'p9', '2')
insert into works_on values('zhangzi2_essn', 'p10', '3')
insert into works_on values('zhangzi3_essn', 'p8', '1')
insert into works_on values('zhangzi3_essn', 'p9', '4')
insert into works_on values('zhangzi3_essn', 'p10', '4')
insert into works_on values('zhangzi4_essn', 'p8', '4')
insert into works_on values('zhangzi4_essn', 'p9', '1')
insert into works_on values('zhangzi4_essn', 'p10', '2')
insert into works_on values('zhangzi5_essn', 'p8', '4')
insert into works_on values('zhangzi5_essn', 'p9', '5')
insert into works_on values('zhangzi5_essn', 'p10', '1')
insert into works_on values('zhangzi6_essn', 'p8', '3')
insert into works_on values('zhangzi6_essn', 'p9', '2')
insert into works_on values('zhangzi6_essn', 'p10', '3')
insert into works_on values('zhangzi7_essn', 'p8', '2')
insert into works_on values('zhangzi7_essn', 'p9', '5')
insert into works_on values('zhangzi7_essn', 'p10', '2')
insert into works_on values('zhangzi8_essn', 'p8', '3')
insert into works_on values('zhangzi8_essn', 'p9', '6')
insert into works_on values('zhangzi8_essn', 'p10', '1')
insert into works_on values('zhangzi9_essn', 'p8', '2')
insert into works_on values('zhangzi9_essn', 'p9', '3')
insert into works_on values('zhangzi9_essn', 'p10', '3')



select * from employee
select * from departmemt
select * from project
select * from works_on



select a.ename from employee a, project b, works_on c where a.essn=c.essn and b.pno=c.pno and b.pname='SQLProject'

select a.ename, a.address from employee a inner join departmemt b where a.dno=b.dno and b.dname='ResearchDepartment' and a.salary<3000

select ename from employee where ename not in (select a.ename from employee a, works_on b where a.essn=b.essn and b.pno='p1')

select a.ename, b.dname from employee a, departmemt b where a.dno=b.dno and a.superssn in (select essn from employee where ename='张红')

select a.essn from employee a, works_on b where a.essn=b.essn and b.pno='p2' and a.essn in (
    select a.essn from employee a, works_on b where a.essn=b.essn and b.pno='p1'
)

select a.ename, a.essn from employee a natural join works_on b group by a.ename having count(b.pno)=(select count(*) from project)

select a.dname from departmemt a natural join employee b group by a.dname having avg(b.salary)<3000

select a.ename from employee a natural join works_on b group by a.ename having sum(b.hours)<=8 and count(b.pno)>=3

select a.dno, sum(a.salary)/sum(b.hours) from employee a natural join works_on b group by a.dno
