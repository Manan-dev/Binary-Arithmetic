#include <cstdio>

int Multiply(int leftop, int rightop);
int Add(int leftop, int rightop);
int Sub(int leftop, int rightop);
int Twos(int op);
int Div(int leftop, int rightop);
int Mod(int leftop, int rightop);

int main(int argc, char *argv[])
{
    int left, right, result;
    char op;

    if (4 != argc)
    {
        printf("Usage: %s <left> <op> <right>\n", argv[0]);
        return -1;
    }
    sscanf(argv[1], "%d", &left);
    sscanf(argv[2], "%c", &op);
    sscanf(argv[3], "%d", &right);

    switch (op)
    {
    case 'm':
    case 'x':
    case '*':
        result = Multiply(left, right);
        break;
    case 'a':
    case '+':
        result = Add(left, right);
        break;
    case 's':
    case '-':
        result = Sub(left, right);
        break;
    case 'd':
    case '/':
        result = Div(left, right);
        break;
    case '%':
        result = Mod(left, right);
        break;
    default:
        result = -11111111;
        break;
    }
    printf("%d\n", result);
    return 0;
}

//Write your functions here
int Add(int leftop, int rightop) {
    int result = 0;           // Dylan Lee's approach
    int carryIn = 0;
    int l, r, total;

    for(int i = 0; i < 32; i++) {
        l = (leftop >> i) & 1;   // mask it with 1 to find if its negative
        r = (rightop >> i) & 1;
        total = (l ^ r) ^ carryIn;   // if either is positive then add to total
        carryIn = (l & r) | (l & carryIn) | (r &carryIn); 
        result = result | total << i;
    }
     return result;
}


int Twos(int op){
    return Add(~op, 1);
}

int Sub(int leftop, int rightop){    
    return Add(Twos(rightop), leftop);
    // return Add(leftop, Twos(rightop));   // Twos complement rightop then use the add function to add the leftop 
}

int Multiply(int leftop, int rightop){
    int result = 0;             // Chris's write up
    int negative = ((leftop >> 31) & 1) ^ ((rightop >> 31) & 1);  // uses XOR and decides if its negative

    if (negative){  // if its negative then use twos complement to flip signs
        leftop = Twos(leftop);
        rightop = Twos(rightop);
    }

    for (int i = 0; i < 32; i ++){
        if (rightop >> i & 1){     // right shift by index
            result = Add(result, leftop << i);  // then add and shitft left by index
        }
    }
    return result;
}

int Div(int leftop, int rightop){       
    int leftSign = ((leftop >> 31) & 1);
    int rightSign = ((rightop >> 31) & 1);  // Zach explained logic
    if (rightSign && leftSign){        // Samantha kind heartedly helped debug
        rightop = Twos(rightop);  // if rightop and leftop is negative, flip sign
        leftop = Twos(leftop);
    }else if (leftSign){
        leftop = Twos(leftop); // if leftop is negative, flip sign
    }else if (rightSign){ // if rightop is negative, flip sign
        rightop = Twos(rightop);
    }
    int quotient;  
    int i;
    if (rightop == 0){
        return 0;
    }
    quotient = 0;
    long R = leftop;
    long D = static_cast <long> (rightop) << 32; // type cast to long because of overflow

     for (i = 31; i  >= 0; i--){
        R = (R << 1) - D;
        if (R >= 0){
            quotient |= 1 << i;
        } else{
            R += D;
        }
     }
    if (leftSign ^ rightSign){  // if either was negative then flip quotient's sign
         quotient = Twos(quotient);
    }
    return quotient;
}


int Mod(int leftop, int rightop){
    int leftSign = ((leftop >> 31) & 1);
    int rightSign = ((rightop >> 31) & 1);
    if (rightSign && leftSign){
        rightop = Twos(rightop);  // if rightop and leftop is negative, flip sign
        leftop = Twos(leftop);
    }else if (leftSign){
        leftop = Twos(leftop); // if leftop is negative, flip sign
    }else if (rightSign){ // if rightop is negative
        rightop = Twos(rightop);
    }

    int remainder = 0;       
    int quotient = 0;
    int i;
    if (rightop == 0){
    remainder = 0;
    quotient =0;
    }
    remainder = 0;
    quotient = 0;
    long R = leftop;
    long D = static_cast <long> (rightop) << 32; // type cast to handle overflow

     for (i = 31; i  >= 0; i--){
        R = (R << 1) - D;
        if (R >= 0){
            quotient |= 1 << i;
        } else{
            R += D;
        }
     }
    remainder = (R >> 32);
    if (leftSign){  // if left was negative then flip quotient's sign
        remainder = Twos(remainder);
    }
     return remainder;  // returns remainder instead of quotient
}

