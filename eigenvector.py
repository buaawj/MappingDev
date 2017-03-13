import numpy as np

mymat = np.mat([[3/100, 3/100, 47/150, 47/150, 47/150],
[1/5, 1/5, 1/5, 1/5, 1/5],
[3/100, 22/25, 3/100, 3/100, 3/100],
[91/200, 3/100, 91/200, 3/100, 3/100],
[91/200, 3/100, 3/100, 91/200, 3/100]])

print (mymat)
print("\n")
eigenValues, eigenVectors = np.linalg.eig(mymat)

print('Eigenvalues {}: \n'.format(eigenValues))
for i in range(len(eigenValues)):
    eigvec_sc = eigenVectors[:,i]
    print('Eigenvector {}: \n{}'.format(i+1, eigvec_sc))
print("\n\n\n\n")
print(np.dot(eigenVectors[:, 0].T, mymat))
arr1 = [ 0.46091656,  0.55825439,  0.46770949,  0.41509445,
   0.29129435]
arr2 = eigenVectors[:, 0]
for i in range(len(arr1)):
    print(arr1[i]/arr2[i])
#print(eigenVectors[0, :]);
