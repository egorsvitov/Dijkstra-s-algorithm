import matplotlib.pyplot as plt
import numpy as np

with open('maze_diags.txt', 'r') as f:
    lines = f.readlines()

path = [tuple(map(int, p.strip('()').split(','))) for p in lines[0].split()]
field = np.array([list(map(int, line.split())) for line in lines[1:]])

plt.figure(figsize=(10, 10))
plt.imshow(field, cmap='binary', origin='upper', extent=[0, field.shape[1], field.shape[0], 0])

# Сдвигаем путь на +0.5 чтобы он шел по центрам клеток
x_vals, y_vals = zip(*[(x+0.5, y+0.5) for x, y in path])
plt.plot(x_vals, y_vals, 'b-', linewidth=2, markersize=8, marker='o', markerfacecolor='blue')

plt.grid(True, which='both', color='gray', linewidth=0.5)
plt.xticks(range(field.shape[1] + 1))
plt.yticks(range(field.shape[0] + 1))
plt.xlabel('X')
plt.ylabel('Y')
plt.tight_layout()
plt.show()