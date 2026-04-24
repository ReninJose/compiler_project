char GLOBAL_CHAR = 'm';

void main(){
    int number = 300;
    float decimal = 50505.033;
    char character = 't';
    bool boolean = true;

    if (number != character){
        number = number + 300;
        decimal = number - decimal * 100;
        if (number > decimal){
            GLOBAL_CHAR = 't';
            number = decimal / 100;
        }
        else {
            float flt = 3.14;
            number = decimal + number * 100 * 3.14;
        }
    }
}