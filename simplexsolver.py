from cassowary import SimplexSolver, Variable, WEAK, STRONG, REQUIRED, MEDIUM
import inspect

solver = SimplexSolver()
#print(inspect.getmembers(solver, predicate=inspect.ismethod))

class Layoutable:
    def __init__(self, name, parent):
        self.parent = parent
        self.name = name
        self.x = Variable('x'+name, 0)
        self.y = Variable('y'+name, 0)
        self.width = Variable('width'+name)
        self.height = Variable('height'+name)
        self.layout = None

        self._fixed_width = False
        self._fixed_height = False

        solver.add_constraint(self.x >= 0, REQUIRED)
        solver.add_constraint(self.y >= 0, REQUIRED)
        solver.add_constraint(self.width >= 0, REQUIRED)
        solver.add_constraint(self.height >= 0, REQUIRED)

    def __repr__(self):
        return "{}(x={}, y={}, width={}, height={})".format(
                         self.name, self.x.value, self.y.value, self.width.value, self.height.value)

class Widget(Layoutable):
    MINIMAL, NORMAL, STRETCH = range(3)

    def __init__(self, name, parent=None):
        super().__init__(name, parent)
        self.maxWidth = 100
        self.maxHeight = 100
        self.minWidth = 10
        self.minHeight = 10
        self.margin = 2
        self.policy = self.STRETCH
        self.layout = None

        solver.add_constraint(self.width >= self.minWidth, REQUIRED)
        solver.add_constraint(self.height >= self.minHeight, REQUIRED)
        solver.add_constraint(self.width <= self.maxWidth, REQUIRED)
        solver.add_constraint(self.height <= self.maxHeight, REQUIRED)

    def resize(self, w, h):
        self._fixed_width = False
        self._fixed_height = False
        self.width.value = w
        solver.add_stay(self.width, MEDIUM)
        self.height.value = h
        solver.add_stay(self.height, MEDIUM)

    def setFixedWidth(self, w):
        self.width.value = w
        solver.add_stay(self.width, STRONG, 2)
        self._fixed_width = True

    def setFixedHeight(self, h):
        self.height.value = h
        solver.add_stay(self.height, STRONG, 2)
        self._fixed_height = True


class HLayout(Layoutable):
    TOP, BOTTOM, LEFT, RIGHT, CENTER = range(5)

    def __init__(self, parent):
        super().__init__("layout", parent)
        self.spacing = 2
        self.width = parent.width
        self.height = parent.height
        self.widgets = []
        self._constraints = []

    def addItem(self, new_w, alignment=CENTER):
        new_w.parent = self.parent
        new_w.layout = self
        self.widgets.append(new_w)
        self.invalidate()

    def add_constraint(self, linear_eq, strength=REQUIRED):
        constraint = solver.add_constraint(linear_eq, strength)
        self._constraints.append(constraint)

    def invalidate(self):

        # remove all constraints
        for c in self._constraints:
            solver.remove_constraint(c)
        self._constraints.clear()

        prevItem = None
        length = len(self.widgets)
        # make sure

        for n, item in enumerate(self.widgets, 1):
            nextItem = None
            if n < length:
                nextItem = self.widgets[n]

            # make sure item can't be out of bounds
            if prevItem:
                self.add_constraint(item.x > prevItem.x+prevItem.width+self.spacing, REQUIRED)
            else:
                self.add_constraint(item.x > layout.x+self.spacing, REQUIRED)

            if nextItem:
                self.add_constraint(item.x+item.width < nextItem.x, REQUIRED)
            else:
                self.add_constraint(item.x+item.width < layout.x+layout.width-self.spacing, REQUIRED)

            self.add_constraint(item.y > layout.y+self.spacing, REQUIRED)
            self.add_constraint(item.y+item.height < layout.y+layout.height-self.spacing, REQUIRED)

            # the size can accommodate if not fixed
            if not item._fixed_width:
                solver.add_stay(item.width, WEAK)
            if not item._fixed_height:
                solver.add_stay(item.height, WEAK)

            # if width is not fixed then width should be the same
            if prevItem and not prevItem._fixed_width and not item._fixed_width:
                self.add_constraint(item.width == prevItem.width, WEAK)
            if not prevItem:
                self.add_constraint(item.width == layout.width, WEAK)

            # same with height
            if not item._fixed_height:
                self.add_constraint(item.height == layout.height - (2*self.spacing), MEDIUM)

            prevItem = item

window = Widget("Window")
# for x
solver.add_stay(window.x)
# for y
solver.add_stay(window.y)
window.resize(20, 20)
layout = HLayout(window)



for x in range(5):
    w = Widget("Widget"+str(x))
    #if x == 1:
    #    w.setFixedWidth(20)
    layout.addItem(w)

layout.widgets[1].setFixedWidth(20)
layout.widgets[1].setFixedHeight(30)
layout.widgets[0].setFixedHeight(10)

def print_all():
    print(window)
    print(layout)
    for w in layout.widgets:
        print("\n", w)

print_all()
#window.resize(500, 500);
#print_all()