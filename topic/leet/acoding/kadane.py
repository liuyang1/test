def max_subarray(xs):
    best_sum, current_sum = 0, 0
    for x in xs:
        current_sum = max(0, current_sum + x)
        best_sum = max(best_sum, current_sum)
        print(x, current_sum, best_sum)
    return best_sum


xs = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
print(max_subarray(xs))
