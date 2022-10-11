import sys
import pandas as pd
import PyQt5
from PyQt5.QtCore import QAbstractTableModel, Qt
from PyQt5.QtWidgets import QMessageBox, QLabel, QTableView
from operate_database import *
from PyQt5 import QtCore, QtWidgets, QtCore

from index import IndexWindow
from query import QyeryWindow
from update import UpdateWindow
from indexes import IndexesWindow
from insert import InsertWindow
from fix import FixWindow
from delete import DeleteWindow
from view import ViewWindow


class Index_Window(QtWidgets.QMainWindow, IndexWindow):
    switch_query = QtCore.pyqtSignal()
    switch_update = QtCore.pyqtSignal()
    switch_view = QtCore.pyqtSignal()
    switch_indexes = QtCore.pyqtSignal()

    def __init__(self):
        super(Index_Window, self).__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.goQuery)
        self.pushButton_2.clicked.connect(self.goUpdate)
        self.pushButton_4.clicked.connect(self.goView)
        self.pushButton_3.clicked.connect(self.goIndexes)

    def goQuery(self):
        self.switch_query.emit()

    def goUpdate(self):
        self.switch_update.emit()

    def goView(self):
        self.switch_view.emit()

    def goIndexes(self):
        self.switch_indexes.emit()


class Query_Window(QtWidgets.QMainWindow, QyeryWindow):
    switch_index = QtCore.pyqtSignal()

    def __init__(self):
        super(Query_Window, self).__init__()
        self.setupUi(self)
        self.pushButton_12.clicked.connect(self.goIndex)
        self.pushButton_11.clicked.connect(self.pb11)
        self.pushButton_7.clicked.connect(self.pb7)
        self.pushButton_8.clicked.connect(self.pb8)
        self.pushButton_10.clicked.connect(self.pb10)
        self.pushButton_9.clicked.connect(self.pb9)
        self.pushButton_3.clicked.connect(self.pb3)
        self.pushButton_2.clicked.connect(self.pb2)
        self.pushButton_4.clicked.connect(self.pb4)
        self.pushButton_6.clicked.connect(self.pb6)
        self.pushButton_5.clicked.connect(self.pb5)
        self.pushButton.clicked.connect(self.pb)
        self.view = QTableView()

    def goIndex(self):
        self.switch_index.emit()

    def pb11(self):
        # 查询每个实验室的教师
        ret = queryTeacherPerLab()
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['l_name', 't_id']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb7(self):
        # 查询拥有某个头衔的教师信息
        honor_name = self.lineEdit_9.text()
        print(honor_name)
        ret = queryTeacherOwnHonor(honor_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['t_id', 't_name', 't_sex', 't_edu', 'd_id', 'l_name', 'c_id']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb8(self):
        # 查询学生的指导教师
        stu_name = self.lineEdit_8.text()
        ret = queryTeacherOfStudent(stu_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['t_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb10(self):
        # 查询某一领域的教师
        domain_name = self.lineEdit_7.text()
        ret = queryTeacherOfDomain(domain_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['t_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb9(self):
        # 查询每个实验室的教师人数
        ret = queryTeacherNumPerLab()
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['l_name', 'num']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb3(self):
        # 查询教师详细信息
        t_id = self.lineEdit.text()
        ret = queryTeacherInfo(t_id)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['t_id', 't_name', 't_sex', 't_edu', 'd_id', 'l_name', 'c_id']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb2(self):
        # 查询教师指导的学生
        teacher_name = self.lineEdit_2.text()
        ret = queryStudentOfTeacher(teacher_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['s_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb4(self):
        # 查询教师的论文
        teacher_name = self.lineEdit_3.text()
        ret = queryPaperOfTeacher(teacher_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['p_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb6(self):
        # 查询论文的作者
        paper_name = self.lineEdit_4.text()
        ret = queryPaperAuthor(paper_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['t_id']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb5(self):
        # 查询教师研究的领域
        teacher_name = self.lineEdit_5.text()
        ret = queryDomainOfTeacher(teacher_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['do_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()

    def pb(self):
        # 查询教师教授的课程
        teacher_name = self.lineEdit_6.text()
        ret = queryCourseOfTeacher(teacher_name)
        if ret is None:
            ret = 'no data'
            QMessageBox.information(self, '查询结果', ret, QMessageBox.Cancel, QMessageBox.Cancel)
        else:
            df = pd.DataFrame(list(ret))
            df.columns = ['c_id', 'c_name']
            model = PdTable(df)
            self.view.setModel(model)
            self.view.setWindowTitle('Result')
            self.view.resize(1600, 1000)
            self.view.setAlternatingRowColors(True)
            self.view.show()


class Update_Window(QtWidgets.QMainWindow, UpdateWindow):
    switch_index = QtCore.pyqtSignal()
    switch_insert = QtCore.pyqtSignal()
    switch_fix = QtCore.pyqtSignal()
    switch_delete = QtCore.pyqtSignal()

    def __init__(self):
        super(Update_Window, self).__init__()
        self.setupUi(self)
        self.pushButton_2.clicked.connect(self.goIndex)
        self.pushButton.clicked.connect(self.goInsert)
        self.pushButton_3.clicked.connect(self.goFix)
        self.pushButton_4.clicked.connect(self.goDelete)

    def goIndex(self):
        self.switch_index.emit()

    def goInsert(self):
        self.switch_insert.emit()

    def goDelete(self):
        self.switch_delete.emit()

    def goFix(self):
        self.switch_fix.emit()


class Insert_Window(QtWidgets.QMainWindow, InsertWindow):
    switch_update = QtCore.pyqtSignal()

    def __init__(self):
        super(Insert_Window, self).__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.goUpdate)
        self.pushButton_2.clicked.connect(self.insert_teacher)
        self.pushButton_3.clicked.connect(self.insert_paper)
        self.pushButton_4.clicked.connect(self.insert_write_paper)

    def goUpdate(self):
        self.switch_update.emit()

    def insert_teacher(self):
        t_id = self.lineEdit.text()
        t_name = self.lineEdit_2.text()
        t_sex = self.lineEdit_4.text()
        if t_sex != 'm' and t_sex != 'f' and t_sex != '':
            QMessageBox.information(self, '结果', 'sex error.', QMessageBox.Cancel, QMessageBox.Cancel)
            return
        t_edu = self.lineEdit_5.text()
        d_id = self.lineEdit_6.text()
        l_name = self.lineEdit_7.text()
        course_id = self.lineEdit_8.text()
        course_name = self.lineEdit_3.text()
        course_hours = self.lineEdit_13.text()
        if t_id == '' or t_name == '' or d_id == '' or l_name == '' or course_id == '':
            QMessageBox.information(self, '结果', 'wrong.', QMessageBox.Cancel, QMessageBox.Cancel)
            return
        data1 = (t_id, t_name, t_sex, t_edu, d_id, l_name, course_id)
        data2 = (course_id, course_name, course_hours)
        try:
            insert_table_data('course', data2)
            insert_table_data('teacher', data1)
        except Exception as e:
            if 'foreign key' in str(e):
                info = 'foreign key error.'
            elif 'Duplicate' in str(e):
                info = 'primary key error.'
            else:
                info = 'error.'
            QMessageBox.information(self, '结果', info, QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '插入成功', QMessageBox.Cancel, QMessageBox.Cancel)

    def insert_paper(self):
        paper_name = self.lineEdit_9.text()
        paper_type = self.lineEdit_10.text()
        if paper_name == '' or paper_type == '':
            QMessageBox.information(self, '结果', 'wrong.', QMessageBox.Cancel, QMessageBox.Cancel)
            return
        data = (paper_name, paper_type)
        try:
            insert_table_data('paper', data)
        except Exception as e:
            if 'Duplicate' in str(e):
                info = 'primary key error.'
            else:
                info = 'error'
            QMessageBox.information(self, '结果', info, QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '插入成功', QMessageBox.Cancel, QMessageBox.Cancel)

    def insert_write_paper(self):
        t_id = self.lineEdit_12.text()
        paper_name = self.lineEdit_11.text()
        if t_id == '' or paper_name == '':
            QMessageBox.information(self, '结果', 'wrong.', QMessageBox.Cancel, QMessageBox.Cancel)
            return
        data = (t_id, paper_name)
        try:
            insert_table_data('write_paper', data)
        except Exception as e:
            if 'foreign key' in str(e):
                info = 'foreign key error.'
            else:
                info = 'error'
            QMessageBox.information(self, '结果', info, QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '插入成功', QMessageBox.Cancel, QMessageBox.Cancel)


class Fix_Window(QtWidgets.QMainWindow, FixWindow):
    switch_update = QtCore.pyqtSignal()

    def __init__(self):
        super(Fix_Window, self).__init__()
        self.setupUi(self)
        self.pushButton_2.clicked.connect(self.goUpdate)
        self.pushButton.clicked.connect(self.fix_teacher_domain)

    def goUpdate(self):
        self.switch_update.emit()

    def fix_teacher_domain(self):
        t_id = self.lineEdit.text()
        pre_do_id = self.lineEdit_3.text()
        new_do_id = self.lineEdit_2.text()
        try:
            update_table_data('research', 'do_id', pre_do_id, 'do_id', new_do_id)
        except Exception as e:
            print(e)
            if 'foreign key' in str(e):
                info = 'foreign key error.'
            else:
                info = 'error'
            QMessageBox.information(self, '结果', info, QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '修改成功', QMessageBox.Cancel, QMessageBox.Cancel)


class Delete_Window(QtWidgets.QMainWindow, DeleteWindow):
    switch_update = QtCore.pyqtSignal()

    def __init__(self):
        super(Delete_Window, self).__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.goUpdate)
        self.pushButton_2.clicked.connect(self.delete_teacher)

    def goUpdate(self):
        self.switch_update.emit()

    def delete_teacher(self):
        t_id = self.lineEdit.text()
        try:
            queryCourseOfTeacher(queryTeacherInfo(t_id=t_id)[0][1])
            delete_table_data('research', 't_id', t_id)
            delete_table_data('write_paper', 't_id', t_id)
            delete_table_data('receive_honor', 't_id', t_id)
            delete_table_data('student', 't_id', t_id)
            delete_table_data('teacher', 't_id', t_id)
        except Exception as e:
            if 'NoneType' in str(e):
                info = 'not exist.'
            else:
                info = 'error.'
            QMessageBox.information(self, '结果', info, QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '删除成功', QMessageBox.Cancel, QMessageBox.Cancel)


class View_Window(QtWidgets.QMainWindow, ViewWindow):
    switch_table = QtCore.pyqtSignal()
    switch_index = QtCore.pyqtSignal()

    def __init__(self):
        super(View_Window, self).__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.goIndex)
        self.pushButton_2.clicked.connect(self.go_Teacher_Info_Table)
        self.pushButton_3.clicked.connect(self.go_Honor_Table)
        self.view = QTableView()

    def goIndex(self):
        self.switch_index.emit()

    def go_Teacher_Info_Table(self):
        ret = view_Teacher_Info()
        df = pd.DataFrame(list(ret))
        df.columns = ['t_id', 't_name', 't_sex', 't_edu', 'd_id', 'l_name', 'c_id']
        model = PdTable(df)
        self.view.setModel(model)
        self.view.setWindowTitle('Teacher Info')
        self.view.resize(1600, 1000)
        self.view.setAlternatingRowColors(True)
        self.view.show()

    def go_Honor_Table(self):
        ret = view_Honor()
        df = pd.DataFrame(list(ret))
        df.columns = ['t_id', 'h_title']
        model = PdTable(df)
        self.view.setModel(model)
        self.view.setWindowTitle('Honor')
        self.view.resize(1600, 1000)
        self.view.setAlternatingRowColors(True)
        self.view.show()


class Indexes_Window(QtWidgets.QMainWindow, IndexesWindow):
    switch_index = QtCore.pyqtSignal()

    def __init__(self):
        super(Indexes_Window, self).__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.goIndex)
        self.pushButton_2.clicked.connect(self.go_Teacher_Name_Index)
        self.pushButton_3.clicked.connect(self.go_Course_Name_Index)
        self.pushButton_4.clicked.connect(self.delete_Teacher_Name_Index)
        self.pushButton_5.clicked.connect(self.delete_Course_Name_Index)

    def goIndex(self):
        self.switch_index.emit()

    def go_Teacher_Name_Index(self):
        try:
            create_teacher_name_index()
        except Exception as e:
            QMessageBox.information(self, '结果', str(e), QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '创建成功', QMessageBox.Cancel, QMessageBox.Cancel)

    def go_Course_Name_Index(self):
        try:
            create_course_name_index()
        except Exception as e:
            QMessageBox.information(self, '结果', str(e), QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '创建成功', QMessageBox.Cancel, QMessageBox.Cancel)

    def delete_Teacher_Name_Index(self):
        try:
            delete_teacher_name_index()
        except Exception as e:
            QMessageBox.information(self, '结果', str(e), QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '删除成功', QMessageBox.Cancel, QMessageBox.Cancel)

    def delete_Course_Name_Index(self):
        try:
            delete_course_name_index()
        except Exception as e:
            QMessageBox.information(self, '结果', str(e), QMessageBox.Cancel, QMessageBox.Cancel)
            return
        QMessageBox.information(self, '结果', '删除成功', QMessageBox.Cancel, QMessageBox.Cancel)


class Controller:
    def __init__(self):
        self.index = Index_Window()
        self.query = Query_Window()
        self.update = Update_Window()
        self.view = View_Window()
        self.indexes = Indexes_Window()
        self.insert = Insert_Window()
        self.fix = Fix_Window()
        self.delete = Delete_Window()

    def show_index(self):
        self.query.close()
        self.update.close()
        self.view.close()
        self.indexes.close()
        self.index.switch_query.connect(self.show_query)
        self.index.switch_update.connect(self.show_update)
        self.index.switch_view.connect(self.show_view)
        self.index.switch_indexes.connect(self.show_indexes)
        self.index.show()

    def show_query(self):
        self.index.close()
        self.query.switch_index.connect(self.show_index)
        self.query.show()

    def show_update(self):
        self.index.close()
        self.insert.close()
        self.fix.close()
        self.delete.close()
        self.update.switch_index.connect(self.show_index)
        self.update.switch_insert.connect(self.show_insert)
        self.update.switch_fix.connect(self.show_fix)
        self.update.switch_delete.connect(self.show_delete)
        self.update.show()

    def show_view(self):
        self.index.close()
        self.view.switch_index.connect(self.show_index)
        self.view.show()

    def show_indexes(self):
        self.index.close()
        self.indexes.switch_index.connect(self.show_index)
        self.indexes.show()

    def show_insert(self):
        self.update.close()
        self.insert.switch_update.connect(self.show_update)
        self.insert.show()

    def show_fix(self):
        self.update.close()
        self.fix.switch_update.connect(self.show_update)
        self.fix.show()

    def show_delete(self):
        self.update.close()
        self.delete.switch_update.connect(self.show_update)
        self.delete.show()


class PdTable(QAbstractTableModel):
    def __init__(self, data):
        QAbstractTableModel.__init__(self)
        self._data = data

    def rowCount(self, parent=None):
        return self._data.shape[0]

    def columnCount(self, parent=None):
        return self._data.shape[1]

    # 显示数据
    def data(self, index, role=Qt.DisplayRole):
        if index.isValid():
            if role == Qt.DisplayRole:
                return str(self._data.iloc[index.row(), index.column()])
        return None

    # 显示行和列头

    def headerData(self, col, orientation, role):
        if orientation == Qt.Horizontal and role == Qt.DisplayRole:
            return self._data.columns[col]
        elif orientation == Qt.Vertical and role == Qt.DisplayRole:
            return self._data.axes[0][col]
        return None


def main():
    app = QtWidgets.QApplication(sys.argv)
    controller = Controller()
    controller.show_index()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
