# Framework for IEEE course final project
# Fan Cheng, 2021

import random
import copy

class Matrix:
	def __init__(self, dim, data=None, init_value=0):
		'''
		1. Matrix有两个属性，self.dim为其维数，为(m, n)。
		2. self.data是其数据，用一个二维嵌套列表表示。选择合适的实现方式初始化data。如果data=None，
		   你需要自己初始化self.data为m*n的[[]]。否则用参数提供的data
		3. init_value为data=None情况下的初始赋值
		'''
		self.dim = dim
		m, n = dim
		if data == None:
			self.data = [[init_value]*n for i in range(m)] 
		else:
			self.data = data		
	

	def set_item(self, indexi, indexj, value):
		# set the data[indexi][indexj] = value
		# you may also try to implement [][] in Matrix
		self.data[indexi][indexj] = value
    

	def get_item(self, indexi, indexj):
		# return value of the data[indexi][indexj] 
		# you may also try to implement [][] in Matrix
		return self.data[indexi][indexj]		
	

	def shape(self):
		'''
		返回矩阵的形状 dim
		'''
		return self.dim


	def reshape(self, newdim):
		'''
		将矩阵从(m,n)维拉伸为newdim=(m1,n1)
		'''
		m1, n1 = newdim
		data = self.data
		self.dim = newdim
		self.data = [[0]*n1 for i in range(m1)]
		lst = []
		for i in data:
			lst += i
		j = 0
		for x in range(m1):
			for y in range(n1):
				self.data[x][y] = lst[j]
				j += 1
		return self


	def dot(self, other):
		'''
		矩阵乘法:矩阵乘以矩阵
		'''
		m, n = self.dim
		p, q = other.dim
		Matrix.T(other)
		lst = Matrix((m, q))
		for i in range(m):
			for j in range(m):
				for x, y in zip(self.data[i], other.data[j]):
					lst.data[i][j] += x*y
		return lst.data


	def T(self):
		'''
		矩阵的转置
		'''
		n, m = self.dim
		newdim = m, n
		self.dim = newdim 

		newdata = [[0]*n for i in range(m)]
		for i in self.data:
			for j in range(len(i)):
				newdata[j] [self.data.index(i)] = i[j]
		self.data = newdata
		return self


	def sum(self, axis=0): #axis = 0, 1, 2
		'''
		根据axis的值，计算矩阵的和。其中
		axis=0:计算所有元素的和
		axis=1:根据横坐标计算每列的和
		axis=2:根据纵坐标计算每行的和
		'''
		m, n = self.dim
		if axis == 0:
			sum = 0
			for i in self.data:
				for j in i:
					sum += j
			return sum
		elif axis == 1:
			other = Matrix.copy(self)
			Matrix.T(other)
			return Matrix.sum(other, 2)
		else :
			lst = [0]*m
			for i in self.data:
				for j in i:
					lst[self.data.index(i)] += j
			return lst
				

	def copy(self):
		'''
		返回matrix的一个备份
		'''
		return Matrix(self.dim, self.data)


	def Kronecker_product(self, other):
		'''
		计算两个矩阵的Kronecker积，具体定义可以搜索
		'''
		m, n = self.dim
		p, q = other.dim
		kplst = Matrix((1,m*p*n*q))
		a = 0
		for i in range(m):
			for x in range(p):
				for j in range(n):
					for y in range(q):
						kplst.data[0][a] = self.data[i][j] * other.data[x][y]
						a += 1
		kplst.reshape((m*p,n*q))
		return kplst.data


	def concatenate(self, other):
		'''
		将两个矩阵连接起来:[[1,2],[3,4]] [[5,6],[7,8]] -> [[1,2,5,6],[3,4,7,8]]
		'''
		lst = []
		for i, j in zip(self.data, other.data):
			lst.append(i + j)
		return lst


	def __pow__(self, n):
		'''
		矩阵的n次幂，n为自然数
		'''
		if n == 0:
			other = Matrix(dim=self.dim)
			m, q = other.dim
			for i in range(m):
				other.set_item(i,i,1)
			return other
		elif n == 1:
			return self.data
		else:
			pow = self.copy()
			while n > 1:
				n -= 1
				pow.data = Matrix.dot(self, pow)
			return pow


	def __add__(self, other):
		'''
		两个矩阵相加
		'''
		m, n = self.dim
		sum = Matrix(self.dim)
		for i in range(m):
			for j in range(n):
				sum.data[i][j] = self.data[i][j] + other.data[i][j]
		return sum


	def __sub__(self, other):
		'''
		两个矩阵相减
		'''
		m, n = self.dim
		dif = Matrix(self.dim)
		for i in range(m):
			for j in range(n):
				dif.data[i][j] = self.data[i][j] - other.data[i][j]
		return dif


	def __mul__(self, other):
		'''
		两个矩阵 对应位置 元素  相乘
		[[1,2]]*[[3, 4]] -> [[3,8]]

		!!!不是矩阵乘法dot
		'''
		m, n = self.dim
		pro = Matrix(self.dim)
		for i in range(m):
			for j in range(n):
				pro.data[i][j] = self.data[i][j] * other.data[i][j]
		return pro


	def __len__(self):
		'''
		返回矩阵元素的数目
		'''
		m, n = self.dim
		return m * n 


	def __str__(self):
		'''
		按照 
		[[  0   1   4   9  16  25  36  49]
 		 [ 64  81 100 121 144 169 196 225]
 		 [256 289 324 361 400 441 484 529]]
 		的格式将矩阵表示为一个 字符串
 		!!! 注意返回值是字符串
		'''
		str1 = '['
		for i in range(len(self.data)):
			if i == len(self.data) - 1:
				str1 +='['
				for j in self.data[i]:
					str1 += '{:>4}'.format(j)
				str1 +=']'
			else:
				str1 +='['
				for j in self.data[i]:
					str1 += '{:>4}'.format(j)
				str1 += ']'+'\n '
		return str1 + ']'


	def det(self):
		'''
		矩阵行列式的值
		'''
		m, n = self.dim
		if m == 1 and n == 1:
			return self.data[-1][-1]
		else:
			sum = 0
			for i in range(len(self.data[0])):
				co = self.data[1:]
				data = copy.deepcopy(co)
				for j in range(len(data)):
					data[j].pop(i)
				other = Matrix((m-1,n-1),data)
				sum += Matrix.det(other) * ((-1) ** i) * self.data[0][i]
			return sum


# 以下几个功能可以选择性实现。
	def inverse(self):
		'''
		矩阵的逆矩阵
		'''
		company = Matrix(self.dim)
		m, n = self.dim
		if Matrix.det(self) == 0:
			return None
		else:
			selfdet = Matrix(self.dim,None,1/Matrix.det(self))
			if m == 1 and n == 1:
				return 1/self.data[1][1]
			else:
				for k in range(len(self.data)):
					for i in range(len(self.data[k])):
						bo = copy.deepcopy(self.data)
						bo.pop(k)
						data = copy.deepcopy(bo)
						for j in range(len(data)):
							data[j].pop(i)
						other = Matrix((m-1,n-1),data)
						company.data[i][k] = Matrix.det(other) * ((-1) ** (i + k))
				return company * selfdet


	def rank(self):
		'''
		矩阵的秩
		'''
		m, n = self.dim
		bo = copy.deepcopy(self.data)
		div1 = bo[0][0]
		for i in range(len(bo[0])):
			bo[0][i] /= div1
		for j in bo[1:]:
			div2 = j[0]
			for x, y in zip(range(len(j)),range(len(bo[0]))):
				j[x] = j[x] - bo[0][y] * div2
		if m == 2 or n == 2:
			pass
		else:
			co = bo[1:]
			data = copy.deepcopy(co)
			for j in range(len(data)):
				data[j].pop(0)
			other = Matrix((m-1,n-1),data)
			Matrix.rank(other)
		for k in bo:
			if k.count(0) == len(k):
				bo.remove(k)
		return len(bo)


def narray(dim, init_value=0): # dim为2维(,), init_value为矩阵元素初始值
	'''
	返回一个matrix，维数为dim，初始值为init_value
	'''
	return Matrix(dim, None, init_value)


def arange(start, end, step=1):
	'''
	返回一个1*n 的 narray
	'''
	data = [list(range(start, end, step))]
	return Matrix((1,(end - start) // step), data)


def zeros(dim):
	'''
	返回一个维数为dim 的全0 narray
	'''
	return narray(dim)


def zeros_like(matrix):
	'''
	返回一个维数和matrix一样 的全0 narray
	'''
	return narray(matrix.dim)


def ones(dim):
	'''
	返回一个维数为dim 的全1 narray
	'''
	return narray(dim,1)


def ones_like(matrix):
	'''
	返回一个维数和matrix一样 的全1 narray
	'''
	return narray(matrix.dim,1)


def nrandom(dim):
	'''
	返回一个维数为dim 的随机 narray
	'''
	return narray(dim, random.random())


def nrandom_like(matrix):
	'''
	返回一个维数和matrix一样 的随机 narray
	'''
	return narray(matrix.dim, random.random())


