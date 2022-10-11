select * from course
select * from department
select * from domain
select * from honor
select * from lab
select * from paper
select * from teacher
select * from student
select * from receive_honor
select * from write_paper
select * from research

select * from teacher where t_id='t1'
select * from teacher where t_name='tname1'

#查询有头衔的教师信息
select * from teacher where t_id in(
    select t.t_id from teacher t, receive_honor rh where t.t_id=rh.t_id
)

#查询有某个头衔的教师信息
select * from teacher where t_id in(
    select t.t_id from teacher t, receive_honor rh where t.t_id=rh.t_id and rh.h_title='outstanding youth'
)

#查询各个实验室拥有的教师人数
select t.l_name, count(*) from teacher t, lab l where t.l_name=l.l_name group by l.l_name

#查询各个实验室的教师，按照人数升/降序排列
select t.l_name, t.t_id from teacher t order by (
    select count(*) from teacher t, lab l where t.l_name=l.l_name group by l.l_name having l.l_name=t.l_name
) asc

#查询论文作者
select t_id from write_paper where p_name='paper a'

#查询教师论文
select b.p_name from teacher a, write_paper b where a.t_id=b.t_id and a.t_name='tname7'

#查询教师教授的课程
select b.c_id, b.c_name from teacher a, course b where a.c_id=b.c_id and a.t_name='tname1'

#查询教师的研究方向
select b.do_name from teacher a, domain b, research c where a.t_id=c.t_id and b.do_id=c.do_id and a.t_name='tname1'

#查询研究方向拥有的教师
select a.t_name from teacher a, domain b, research c where a.t_id=c.t_id and b.do_id=c.do_id and b.do_name='AI'

#查询教师指导的学生
select b.s_name from teacher a, student b where a.t_id=b.t_id and a.t_name='tname1'

#查询学生的指导教师
select a.t_name from teacher a, student b where a.t_id=b.t_id and b.s_name='sname3'

create view view_a as select * from teacher where t_id in(
    select t.t_id from teacher t, receive_honor rh where t.t_id=rh.t_id
)
select * from view_a

drop view tname1info

create view teacher_view as select * from teacher
create view receive_honor_view as select * from receive_honor

select * from teacher_view
select * from receive_honor_view

create index teacher_name_index on teacher(t_name)
drop index teacher_name_index on teacher