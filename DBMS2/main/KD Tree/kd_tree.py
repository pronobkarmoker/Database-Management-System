import numpy as np

class KDNode:
    def __init__(self, point, left=None, right=None):
        self.point = point
        self.left = left
        self.right = right
        self.axis = None

class KDTree:
    def __init__(self):
        self.root = None
        
    def insert(self, point):
        def _insert(node, point, depth):
            if node is None:
                node = KDNode(point)
                node.axis = depth % len(point)
                return node
            
            axis = depth % len(point)
            node.axis = axis
            
            if point[axis] < node.point[axis]:
                node.left = _insert(node.left, point, depth + 1)
            else:
                node.right = _insert(node.right, point, depth + 1)
            
            return node
        
        self.root = _insert(self.root, point, 0)
    
    def search_nearest(self, target):
        best = [None]  # to store best point
        best_dist = [float('inf')]  # to store best distance
        
        def _search_nearest(node, target, depth):
            if node is None:
                return
            
            # Calculate current distance
            current_dist = np.sqrt(sum((a - b) ** 2 for a, b in zip(node.point, target)))
            
            # Update best if current is better
            if current_dist < best_dist[0]:
                best_dist[0] = current_dist
                best[0] = node.point
            
            axis = depth % len(target)
            
            # Decide which subtree to search first
            if target[axis] < node.point[axis]:
                first, second = node.left, node.right
            else:
                first, second = node.right, node.left
            
            _search_nearest(first, target, depth + 1)
            
            # Check if we need to search the other subtree
            if abs(target[axis] - node.point[axis]) < best_dist[0]:
                _search_nearest(second, target, depth + 1)
        
        _search_nearest(self.root, target, 0)
        return best[0]

    def print_tree(self):
        def _print_tree(node, level=0, prefix="Root: "):
            if node is not None:
                spacing = "  " * level
                axis_name = "x" if node.axis == 0 else "y"
                print(f"{spacing}{prefix}[{axis_name}] {node.point}")
                if node.left:
                    _print_tree(node.left, level + 1, "L: ")
                if node.right:
                    _print_tree(node.right, level + 1, "R: ")
        
        _print_tree(self.root)


if __name__ == "__main__":
    # read file
    points = []
    with open('points.txt', 'r') as f:
        for line in f:
            x, y = map(float, line.strip().split(','))
            points.append((x, y))
    
    # build KD Tree
    kdtree = KDTree()
    for point in points:
        kdtree.insert(point)
    
    # print tree
    print("KD Tree Structure:")
    # print("-------------------------")
    kdtree.print_tree()
    print("\n")
    # print("-------------------------\n")
    
    # Test searching for nearest neighbor
    target = (4.5, 5.5)
    nearest = kdtree.search_nearest(target)
    
    # print(f"Points in dataset: {points}")
    print(f"Target point: {target}")
    print(f"Nearest neighbor: {nearest}")
    print("\n")
