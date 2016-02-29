from cassowary import SimplexSolver, Variable, WEAK, STRONG, REQUIRED, MEDIUM

solver = SimplexSolver()

class Widget():
    MINIMAL, NORMAL, STRETCH = range(3)

    def __init__(self, name):
        self.x = Variable('x'+name, 0)
        self.y = Variable('y'+name, 0)
        self.width = Variable('width'+name)
        self.height = Variable('height'+name)
        self.realW = 20
        self.realH = 20
        self.maxWidth = 50
        self.maxHeight = 50
        self.minWidth = 10
        self.minHeight = 10
        self.name = name
        self.policy = self.MINIMAL

    def __repr__(self):
        return "{}(x={}, y={}, width={}, height={})".format(
                         self.name, self.x.value, self.y.value, self.width.value, self.height.value)

widgets = []
for x in range(1):
    n = 'Widget' + str(x)
    widgets.append(Widget(n))

window = Widget("Window")
window.x.value = 0
window.y.value = 0
window.width.value = 50
window.height.value = 50
margin = 2
spacing = 2
start_x = 0
start_y = 0

# for x
solver.add_stay(window.x)
solver.add_stay(window.width, STRONG)
solver.add_constraint(window.x >= start_x, strength=REQUIRED)

# for y
solver.add_stay(window.y)
solver.add_stay(window.height, STRONG)
solver.add_constraint(window.y >= start_y, strength=REQUIRED)

for n, w in enumerate(widgets):
    # for x
    solver.add_constraint(w.x > window.x, strength=REQUIRED)
    if w.policy == Widget.NORMAL:
        w.width.value = w.realW
        solver.add_stay(w.width, STRONG)
        solver.add_constraint(w.width >= w.minWidth, strength=STRONG)
    elif w.policy == Widget.MINIMAL:
        solver.add_constraint(w.width == w.minWidth, strength=STRONG)
    solver.add_constraint(w.width <= w.maxWidth, strength=STRONG)

    # for y
    solver.add_constraint(w.y > window.y, strength=REQUIRED)
    if w.policy == Widget.NORMAL:
        w.height.value = w.realH
        solver.add_stay(w.height, MEDIUM)
        solver.add_constraint(w.height >= w.minHeight, strength=STRONG)
    elif w.policy == Widget.MINIMAL:
        solver.add_constraint(w.height == w.minHeight, strength=STRONG)
    solver.add_constraint(w.height <= w.maxHeight, strength=STRONG)


    # left margin
    if n == 0:
        solver.add_constraint(w.x >= window.x+margin, strength=REQUIRED)
        # alignment
    else:
        prev_widget = widgets[n-1]
        solver.add_constraint(w.x >= prev_widget.x+prev_widget.width+1, strength=REQUIRED)
        solver.add_constraint(w.x >= prev_widget.x+prev_widget.width+spacing/2, strength=MEDIUM)

    # top margin
    solver.add_constraint(w.y == window.y+margin, strength=REQUIRED)

    # right margin
    if n+1 == len(widgets):
        solver.add_constraint(w.x+w.width <= window.x+window.width-margin, strength=REQUIRED)
    else:
        next_widget = widgets[n+1]
        solver.add_constraint(w.x+w.width <= next_widget.x+next_widget.width-1, strength=REQUIRED)
        solver.add_constraint(w.x+w.width <= next_widget.x+next_widget.width-spacing/2, strength=MEDIUM)

    # bottom margin
    if w.policy == Widget.MINIMAL:
        solver.add_constraint(w.y+w.height <= window.y+window.height-margin, strength=REQUIRED)
    else:
        solver.add_constraint(w.y+w.height == window.y+window.height-margin, strength=REQUIRED)

# alignment
#center
first_w = widgets[0]
last_w = widgets[len(widgets)-1]
solver.add_constraint(first_w.x-window.x == (window.x+window.width)-(last_w.x+last_w.width))

print(window)
for w in widgets:
    print("\n", w)