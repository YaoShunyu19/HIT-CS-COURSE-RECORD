class Node:  # 定义节点类
    def __init__(self, operation='', value=''):
        self.child = []
        self.operation = operation
        self.value = value

    def __str__(self):
        if self.operation == '':
            return self.value
        return self.operation + ' ' + self.value


def generateTree(query):
    tokens = query.split(' ')
    head = Node()
    cur_node = head
    idx = 0
    while idx < len(tokens):
        token = tokens[idx]
        if token == 'SELECT' or token == 'PROJECTION':
            node = Node()
            cur_node.child.append(node)
            end = tokens.index(']', idx)
            node.operation = token
            node.value = ' '.join(tokens[idx + 2:end])
            cur_node = node
            idx = end + 1
        elif token == 'JOIN':
            node = Node()
            cur_node.child.append(node)
            node.operation = 'JOIN'
            node.child.append(Node(value=tokens[idx - 1]))
            node.child.append(Node(value=tokens[idx + 1]))
            cur_node = node
            idx += 1
        else:
            idx += 1
    return head.child[0]


def printTree(head, sep=''):
    print(sep + str(head))
    for child in head.child:
        printTree(child, sep + '  ')


def optimize(node, value_list=None):
    # value_list:保存SELECT的对象值，用于先进行选择运算时输出
    # 选择运算先做
    if node.operation == 'SELECT':
        node = optimize(node.child[0], node.value.split('&'))
    elif node.operation == 'PROJECTION':  # 不做任何操作，继续进行
        node.child[0] = optimize(node.child[0], value_list)
    elif node.operation == 'JOIN':
        for i in range(len(value_list)):  # 把选择运算放到JOIN后，需要按序进行选择运算和对象值的输出
            temp_node = Node(operation='SELECT', value=value_list[i])
            temp_node.child.append(node.child[i])
            node.child[i] = temp_node
    return node


if __name__ == '__main__':
    query0 = 'SELECT [ ENAME = ’Mary’ & DNAME = ’Research’ ] ( EMPLOYEE JOIN DEPARTMENT )'
    query1 = 'SELECT [ ESSN = ’01’ ] ( PROJECTION [ ESSN, PNAME ] ( WORKS_ON JOIN PROJECT ) )'
    query2 = 'PROJECTION [ BDATE ] ( SELECT [ ENAME = ’John’ & DNAME = ’Research’ ] ( EMPLOYEE JOIN DEPARTMENT ) )'
    head0 = generateTree(query0)
    head1 = generateTree(query1)
    head2 = generateTree(query2)
    printTree(head0)
    print('\n')
    printTree(optimize(head0))
    print('\n\n')
    printTree(head1)
    print('\n')
    printTree(optimize(head1))
    print('\n\n')
    printTree(head2)
    print('\n')
    printTree(optimize(head2))
