from math import log

def evaluate_submission(labels, ranks, k = 10):

    nq = len(labels) # Number of user_id_a
    assert len(ranks) == nq, 'Expected %d lines, but got %d.'%(nq, len(ranks))

    ndcg10 = 0.0
    ndcg20 = 0.0

    for i in range(nq):

        l = [int(x) for x in labels[i].split(' ')]

        try:
            r = [int(x) for x in ranks[i].split(' ')]

        except ValueError:
            raise ValueError('Non integer value on line %d'%(i+1))

        nd = len(l)
        assert len(r) == nd, 'Expected %d ranks at line %d, but got %d.'%(nd, i+1, len(r))

        gains = [-1]*nd # must be initialized as -1
        assert max(r) <= nd, 'Ranks on line %d larger than number of documents (%d).'%(i+1, nd)

        assert min(r) >=1, 'Ranks on line %d smaller than 1.'%(i+1)

        for j in range(nd):
            gains[r[j]-1] = 2**l[j] - 1.0

        assert min(gains) >= 0, 'Not all ranks present at line %d.'%(i+1)


        dcg10 = sum([g/log(j+2) for (j, g) in enumerate(gains[:k])])
        dcg20 = sum([g/log(j+2) for (j, g) in enumerate(gains[:(2*k)])])
        gains.sort()
        gains = gains[::-1]

        ideal_dcg10 = sum([g/log(j+2) for (j, g) in enumerate(gains[:k])])
        ideal_dcg20 = sum([g/log(j+2) for (j, g) in enumerate(gains[:(2*k)])])

        if ideal_dcg10 != 0.0:
            ndcg10 += dcg10/ideal_dcg10
        else:
            ndcg10 += 1.0

        if ideal_dcg20 != 0.0:
            ndcg20 += dcg20/ideal_dcg20
        else:
            ndcg20 += 1.0

    return (ndcg10/nq, ndcg20/nq)


labels = []
f_labels = open('labels_train.txt')
while True:
    newline = str(f_labels.readline())
    if len(newline) == 0:
        break
    else:
        labels.append(newline)

ranks = []
import sys

for arg in sys.argv:
	f_ranks = open(arg)

while True:
	newline = str(f_ranks.readline())
	if len(newline) == 0:
		break
	else:
		ranks.append(newline)

ndcg10, ndcg20 = evaluate_submission(labels, ranks, k = 10)
print(ndcg10, ndcg20)




