int early_return(int x) {
    return x;
    int dead1 = 99;
    int dead2 = 100;
}

int always_true_branch(int x) {
    if (1 == 1) {
        return x + 1;
    } else {
        return 0;
    }
}

int always_false_branch(int x) {
    if (0 == 1) {
        return 99;
    } else {
        return x;
    }
}
