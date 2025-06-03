# Software Testing Project
**VUT FIT 2024/25 - IVS Project 1**

C++ project implementing three types of software testing using GoogleTest framework.

## Project parts

### Black Box Testing
Test Red-Black Tree operations without knowing implementation details:
- Insert, Delete, Find operations
- Verify Red-Black Tree axioms

### White Box Testing  
Test hash table implementation with full code access:
- Achieve high code coverage
- Test collision handling

### Test Driven Development
Implement graph class based on existing tests:
- Undirected graph without loops
- Node coloring algorithms

## Build & Run
```bash
cd build
cmake ..
make
ctest -C Debug
```

## Files
- `black_box_tests.cpp` - Red-Black Tree tests
- `white_box_tests.cpp` - Hash table tests  
- `tdd_code.cpp/.h` - Graph implementation

**Score: 17.6/18 points**
