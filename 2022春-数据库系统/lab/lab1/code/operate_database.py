import pymysql

host, user, pwd = 'localhost', 'root', '123456'

department = '''create table department(
             d_id varchar(10) not null comment '院系编号',
             d_name varchar(20) not null comment '院系名称',
             primary key(d_id)
             );
             '''

lab = '''create table lab(
      l_name varchar(20) not null comment '实验室名称',
      l_web varchar(32) comment '实验室网站',
      primary key(l_name)
      );
      '''

domain = '''create table domain(
         do_id varchar(10) not null comment '方向编号',
         do_name varchar(20) not null comment '方向名称',
         primary key(do_id)
         );
         '''

course = '''create table course(
         c_id varchar(10) not null comment '课程代码',
         c_name varchar(20) not null comment '课程名',
         c_hours int not null comment '学时',
         primary key(c_id),
         check (c_hours>0)
         );
         '''

honor = '''create table honor(
        h_title varchar(20) not null comment '头衔名称',
        h_level varchar(10) comment '级别',
        primary key(h_title)
        );
        '''

paper = '''create table paper(
        p_name varchar(64) not null comment '论文名称',
        p_type varchar(20) comment '论文类别',
        primary key(p_name)
        );
        '''

teacher = '''create table teacher(
             t_id varchar(10) not null comment '工号',
             t_name varchar(10) not null comment '姓名',
             t_sex varchar(1) comment '性别',
             t_edu varchar(10) comment '学历',
             d_id varchar(10) not null comment '院系编号',
             l_name varchar(20) not null comment '实验室名称',
             c_id varchar(10) not null comment '开设课程代码',
             primary key(t_id),
             foreign key(d_id) references department(d_id),
             foreign key(l_name) references lab(l_name),
             check (t_sex='m' or t_sex='f' or t_sex='')
             );
            '''

student = '''create table student(
          s_id varchar(10) not null comment '学号',
          s_name varchar(10) not null comment '姓名',
          d_id varchar(10) not null comment '院系编号',
          t_id varchar(10) not null comment '指导教师工号',
          primary key(s_id),
          foreign key(d_id) references department(d_id),
          foreign key(t_id) references teacher(t_id)
          );
          '''

research = '''create table research(
           t_id varchar(10) not null comment '教师工号',
           do_id varchar(10) not null comment '方向编号',
           foreign key(t_id) references teacher(t_id),
           foreign key(do_id) references domain(do_id)
           );
           '''
receive_honor = '''create table receive_honor(
                t_id varchar(10) not null comment '教师工号',
                h_title varchar(20) not null comment '头衔名称',
                foreign key(t_id) references teacher(t_id),
                foreign key(h_title) references honor(h_title)
                );
                '''
write_paper = '''create table write_paper(
        t_id varchar(10) not null comment '作者工号',
        p_name varchar(64) not null comment '论文名称',
        foreign key(t_id) references teacher(t_id),
        foreign key(p_name) references paper(p_name)
        );
        '''

create_table_sql = [department, lab, domain, course, honor, paper, teacher, student, research, receive_honor,
                    write_paper]

department_data = [
    ('d1', 'CS'),
    ('d2', 'SE'),
    ('d3', 'IS')
]

lab_data = [
    ('cv', 'www.cv'),
    ('nlp', 'www.nlp'),
    ('db', 'www.db'),
    ('bi', 'www.bi'),
    ('ss', 'www.ss')
]

domain_data = [
    ('do1', 'AI'),
    ('do2', 'System'),
    ('do3', 'Theory'),
    ('do4', 'Interdisciplinary'),
    ('do5', 'Security'),
    ('do6', 'Robotics')
]

course_data = [
    ('c1', 'ML', 48),
    ('c2', 'DL', 48),
    ('c3', 'CSAPP', 96),
    ('c4', 'CN', 32),
    ('c5', 'DB', 64),
    ('c6', 'PR', 32),
    ('c7', 'DS', 48),
    ('c8', 'Algorithm', 64),
    ('c9', 'SC', 48),
    ('c10', 'DM', 16),
    ('c11', 'GT', 64),
    ('c12', 'MA', 32)
]

honor_data = [
    ('academician', 'A'),
    ('outstanding youth', 'B'),
    ('YR Scholar', 'C')
]

paper_data = [
    ('paper a', 'conference'),
    ('paper b', 'conference'),
    ('paper c', 'conference'),
    ('paper d', 'conference'),
    ('paper e', 'conference'),
    ('paper f', 'conference'),
    ('paper g', 'conference'),
    ('paper h', 'conference'),
    ('paper i', 'periodicals'),
    ('paper j', 'periodicals'),
    ('paper k', 'periodicals'),
    ('paper l', 'periodicals'),
    ('paper m', 'periodicals'),
    ('paper n', 'periodicals'),
    ('paper o', 'patent'),
    ('paper p', 'patent')
]

teacher_data = [
    ('t1', 'tname1', 'm', 'edu1', 'd1', 'cv', 'c1'),
    ('t2', 'tname2', 'f', 'edu', 'd1', 'cv', 'c2'),
    ('t3', 'tname3', 'm', 'edu1', 'd1', 'cv', 'c3'),
    ('t4', 'tname4', 'f', 'edu1', 'd1', 'nlp', 'c4'),
    ('t5', 'tname5', 'm', 'edu2', 'd1', 'nlp', 'c5'),
    ('t6', 'tname6', 'f', 'edu2', 'd2', 'nlp', 'c6'),
    ('t7', 'tname7', 'm', 'edu3', 'd2', 'db', 'c7'),
    ('t8', 'tname8', 'f', 'edu1', 'd2', 'db', 'c8'),
    ('t9', 'tname9', 'm', 'edu1', 'd2', 'bi', 'c9'),
    ('t10', 'tname10', 'm', 'edu1', 'd3', 'ss', 'c10'),
    ('t11', 'tname11', 'm', 'edu1', 'd3', 'ss', 'c11'),
    ('t12', 'tname12', 'f', 'edu2', 'd3', 'ss', 'c12')
]

student_data = [
    ('s1', 'sname1', 'd1', 't1'),
    ('s2', 'sname2', 'd1', 't1'),
    ('s3', 'sname3', 'd1', 't2'),
    ('s4', 'sname4', 'd1', 't3'),
    ('s5', 'sname5', 'd1', 't4'),
    ('s6', 'sname6', 'd1', 't4'),
    ('s7', 'sname7', 'd2', 't5'),
    ('s8', 'sname8', 'd2', 't6'),
    ('s9', 'sname9', 'd2', 't7'),
    ('s10', 'sname10', 'd2', 't8'),
    ('s11', 'sname11', 'd2', 't9'),
    ('s12', 'sname12', 'd3', 't10'),
    ('s13', 'sname13', 'd3', 't11'),
    ('s14', 'sname14', 'd3', 't12'),
    ('s15', 'sname15', 'd3', 't12')
]

receive_honor_data = [
    ('t1', 'academician'),
    ('t3', 'outstanding youth'),
    ('t10', 'outstanding youth'),
    ('t8', 'YR Scholar'),
    ('t5', 'YR Scholar'),
    ('t7', 'YR Scholar')
]

write_paper_data = [
    ('t1', 'paper a'),
    ('t1', 'paper b'),
    ('t1', 'paper c'),
    ('t1', 'paper d'),
    ('t1', 'paper e'),
    ('t1', 'paper f'),
    ('t1', 'paper g'),
    ('t1', 'paper h'),
    ('t2', 'paper i'),
    ('t2', 'paper j'),
    ('t3', 'paper k'),
    ('t3', 'paper l'),
    ('t3', 'paper m'),
    ('t3', 'paper n'),
    ('t3', 'paper o'),
    ('t4', 'paper p'),
    ('t4', 'paper a'),
    ('t5', 'paper b'),
    ('t5', 'paper c'),
    ('t5', 'paper d'),
    ('t5', 'paper e'),
    ('t6', 'paper f'),
    ('t7', 'paper g'),
    ('t7', 'paper h'),
    ('t8', 'paper i'),
    ('t8', 'paper a'),
    ('t9', 'paper b'),
    ('t10', 'paper a'),
    ('t10', 'paper a'),
    ('t10', 'paper b'),
    ('t11', 'paper d'),
    ('t12', 'paper g'),
    ('t12', 'paper j')
]

research_data = [
    ('t1', 'do1'),
    ('t1', 'do3'),
    ('t1', 'do4'),
    ('t1', 'do6'),
    ('t2', 'do1'),
    ('t3', 'do1'),
    ('t3', 'do4'),
    ('t3', 'do6'),
    ('t4', 'do1'),
    ('t5', 'do1'),
    ('t5', 'do3'),
    ('t6', 'do1'),
    ('t7', 'do2'),
    ('t7', 'do3'),
    ('t8', 'do2'),
    ('t8', 'do3'),
    ('t9', 'do4'),
    ('t10', 'do2'),
    ('t10', 'do3'),
    ('t10', 'do5'),
    ('t11', 'do5'),
    ('t12', 'do2')
]

insert_data = {'department': department_data, 'lab': lab_data, 'domain': domain_data, 'course': course_data,
               'honor': honor_data, 'paper': paper_data, 'teacher': teacher_data, 'student': student_data,
               'receive_honor': receive_honor_data, 'write_paper': write_paper_data, 'research': research_data}


def delete_database(database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd)
    cur = connect.cursor()
    delete_database_sql = 'drop database ' + database + ';'
    cur.execute(delete_database_sql)
    cur.close()
    connect.close()


def create_database():
    connect = pymysql.connect(host=host, user=user, password=pwd)
    cur = connect.cursor()
    create_database_sql = 'create database dblab1;'
    cur.execute(create_database_sql)
    cur.close()
    connect.close()


def create_table(database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    for sql in create_table_sql:
        print(sql)
        cur.execute(sql)
    cur.close()
    connect.close()


def delete_table(table, database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    sql = 'drop table ' + table + ';'
    cur.execute(sql)
    cur.close()
    connect.close()


def initial_table_data(database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    for k, v in insert_data.items():
        for i in v:
            sql = 'insert into %s values ' % k + str(i)
            print(sql)
            cur.execute(sql)
    connect.commit()
    cur.close()
    connect.close()


def create_view(database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    sql1 = 'create view teacher_view as select * from teacher'
    sql2 = 'create view receive_honor_view as select * from receive_honor'
    cur.execute(sql1)
    cur.execute(sql2)
    cur.close()
    connect.close()


def insert_table_data(table, data, database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    if isinstance(data, list):
        for d in data:
            sql = 'insert into %s values ' % table + str(d)
            print(sql)
            cur.execute(sql)
    else:
        sql = 'insert into %s values ' % table + str(data)
        print(sql)
        cur.execute(sql)
    connect.commit()
    cur.close()
    connect.close()


def delete_table_data(table, key, value, database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    sql = 'delete from %s where %s=\'%s\'' % (table, key, value)
    print(sql)
    cur.execute(sql)
    connect.commit()
    cur.close()
    connect.close()


def update_table_data(table, key, value, new_key, new_value, database='dblab1'):
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    sql = 'update %s set %s=\'%s\' where %s=\'%s\'' % (table, new_key, new_value, key, value)
    print(sql)
    cur.execute(sql)
    connect.commit()
    cur.close()
    connect.close()


def queryTeacherInfo(t_id=None, t_name=None, storage_name=None, database='dblab1'):
    if t_id is None and t_name is None:
        sql = 'select * from teacher'
    elif t_id is None and t_name is not None:
        sql = 'select * from teacher where t_name=\'%s\'' % t_name
    elif t_id is not None and t_name is None:
        sql = 'select * from teacher where t_id=\'%s\'' % t_id
    else:
        sql = 'select * from teacher where t_id=\'%s\' and t_name=\'%s\'' % (t_id, t_name)

    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    if storage_name is not None:
        view_sql = 'create view %s as %s' % (storage_name, sql)
        cur.execute(view_sql)
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryTeacherOwnHonor(honor=None, database='dblab1'):
    if honor is None:
        sql = 'select * from teacher where t_id in ' \
              '(select t.t_id from teacher t, receive_honor rh where t.t_id=rh.t_id)'
    else:
        assert isinstance(honor, str)
        sql = 'select * from teacher where t_id in ' \
              '(select t.t_id from teacher t, receive_honor rh ' \
              'where t.t_id=rh.t_id and rh.h_title=\'%s\')' % honor
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryTeacherNumPerLab(database='dblab1'):
    sql = 'select t.l_name, count(*) from teacher t, lab l where t.l_name=l.l_name group by l.l_name'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryTeacherPerLab(order='asc', database='dblab1'):
    assert order == 'asc' or order == 'desc'
    sql = 'select t.l_name, t.t_id from teacher t order by ' \
          '(select count(*) from teacher t, lab l where t.l_name=l.l_name ' \
          'group by l.l_name having l.l_name=t.l_name) %s' % order
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryPaperAuthor(paper_name, database='dblab1'):
    assert isinstance(paper_name, str)
    sql = 'select t_id from write_paper where p_name=\'%s\'' % paper_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryPaperOfTeacher(teacher_name, database='dblab1'):
    assert isinstance(teacher_name, str)
    sql = 'select b.p_name from teacher a, write_paper b where a.t_id=b.t_id and a.t_name=\'%s\'' % teacher_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryCourseOfTeacher(teacher_name, database='dblab1'):
    assert isinstance(teacher_name, str)
    sql = 'select b.c_id, b.c_name from teacher a, course b where a.c_id=b.c_id and a.t_name=\'%s\'' % teacher_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryDomainOfTeacher(teacher_name, database='dblab1'):
    assert isinstance(teacher_name, str)
    sql = 'select b.do_name from teacher a, domain b, research c ' \
          'where a.t_id=c.t_id and b.do_id=c.do_id and a.t_name=\'%s\'' % teacher_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryTeacherOfDomain(domain_name, database='dblab1'):
    assert isinstance(domain_name, str)
    sql = 'select a.t_name from teacher a, domain b, research c ' \
          'where a.t_id=c.t_id and b.do_id=c.do_id and b.do_name=\'%s\'' % domain_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryStudentOfTeacher(teacher_name, database='dblab1'):
    assert isinstance(teacher_name, str)
    sql = 'select b.s_name from teacher a, student b where a.t_id=b.t_id and a.t_name=\'%s\'' % teacher_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def queryTeacherOfStudent(student_name, database='dblab1'):
    assert isinstance(student_name, str)
    sql = 'select a.t_name from teacher a, student b where a.t_id=b.t_id and b.s_name=\'%s\'' % student_name
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def view_Teacher_Info(database='dblab1'):
    sql = 'select * from teacher_view'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def view_Honor(database='dblab1'):
    sql = 'select * from receive_honor_view'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    ret = cur.fetchall()
    cur.close()
    connect.close()
    if not ret:
        print('no data.')
        return None
    return ret


def create_teacher_name_index(database='dblab1'):
    sql = 'create index teacher_name_index on teacher(t_name)'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    cur.close()
    connect.close()


def create_course_name_index(database='dblab1'):
    sql = 'create index course_name_index on course(c_name)'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    cur.close()
    connect.close()


def delete_teacher_name_index(database='dblab1'):
    sql = 'drop index teacher_name_index on teacher'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    cur.close()
    connect.close()


def delete_course_name_index(database='dblab1'):
    sql = 'drop index course_name_index on course'
    connect = pymysql.connect(host=host, user=user, password=pwd, database=database, charset='utf8')
    cur = connect.cursor()
    cur.execute(sql)
    cur.close()
    connect.close()


if __name__ == '__main__':
    delete_database()
    create_database()
    create_table()
    initial_table_data()
    create_view()
    exit()

    # delete_table(table='department')
    # delete_table(table='domain')
    # delete_table(table='lab')
    # delete_table(table='course')
    # delete_table(table='honor')
    # delete_table(table='paper')
    # delete_table(table='teacher')
    # delete_table(table='student')
    # delete_table(table='research')
    # delete_table(table='receive_honor')

    # insert_table_data('domain', ('do_id111', 'long'))
    # update_table_data('domain', 'do_name', 'long', 'do_id', 'do_id123')
    # delete_table_data('domain', 'do_id', 'do_id123')
    # insert_table_data('write_paper', write_paper_data)

    # ret = queryTeacherInfo(t_name='tname1', storage_name='tname1info')
    # print(ret)

    # insert_table_data('teacher', '(\'t1\', \'tname13\', \'f\', \'s\', \'d1\', \'bi\', \'ddd\')')
