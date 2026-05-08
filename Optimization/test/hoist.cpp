int code_hoisting(int x) {
    if (x > 0) {
        int base = 10;
        int result = base + x;
        return result;
    } else {
        int base = 10;
        int result = base - x;
        return result;
    }
}