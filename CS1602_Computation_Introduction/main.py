# Test code for IEEE course final project
# Fan Cheng, 2021

import minimatrix as mm
#Test your code here!

mat = mm.Matrix((4,4),None,0)

mat.set_item(0,0,3)
mat.set_item(0,1,1)
mat.set_item(0,2,2)
mat.set_item(0,3,1)
mat.set_item(1,0,5)
mat.set_item(1,1,2)
mat.set_item(1,2,3)
mat.set_item(1,3,1)
mat.set_item(2,2,3)
mat.set_item(2,3,1)
mat.set_item(3,2,4)
mat.set_item(3,3,1)

'''print('3行2列元素是:',mat.get_item(2,1))

print('矩阵形状:',mat.shape())

mat1 = mat.copy()
mat1.reshape([9,1])
print('变成9行1列:',mat1.data)

print('自己和自己内积:',mat.dot(mat.copy()))

mat2 = mat.copy()
mat2.T()
print('转置一下:',mat2.data)

print('全部加起来:',mat.sum())
print('各列加起来:',mat.sum(1))
print('各行加起来:',mat.sum(2))

print('自己和自己克罗内克积:')
print(mat.Kronecker_product(mat))

print('自己和自己接一起:',mat.concatenate(mat))

print('0次幂:',(mat ** 0).data)
print('3次幂:',(mat ** 3).data)

print('加自己的转置:',(mat + mat2).data)

print('减自己的转置:',(mat - mat2).data)

print('对应乘自己的转置:',(mat * mat2).data)

print('总元素个数:',len(mat))

print('变成字符串:')
print(str(mat))
print(type(str(mat)))

print('行列式:',mat.det())
'''
print('逆（可惜没有）:\n',mat.inverse())
'''
print('秩:',mat.rank())


m24 = mm.arange(0,24)
print('4行6列:',m24.reshape([4,6]).data)
print('24行1列:',m24.reshape([24,1]).data)
print('3行8列:',m24.reshape([3,8]).data)

m0 = mm.zeros([3,3])
print('3行3列全是0:',m0.data)
print('按m24的格式生成0:',mm.zeros_like(m24).data)

m1 = mm.ones([3,3])
print('3行3列全是1:',m1.data)
print('按m24的格式生成1:',mm.ones_like(m24).data)

mrdm = mm.nrandom([3,3])
print('3行3列随机搞:',mrdm.data)
print('按m24的格式随机搞:',mm.nrandom_like(m24).data)
'''