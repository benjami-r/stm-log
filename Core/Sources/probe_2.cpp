

//Unit.RCC.GPIOAenabled;
//Unit.GPIOA.P5.Config(ENABLE, OUTPUT, PUSHPULL, LOW, NOPUPD);

//LED.
//

using uint = unsigned int;
using addr = volatile uint *;

template <addr Address> class Cell {
protected:
    static void write (const uint value) {
        *Address = value;
    }
    static uint read () {
        return *Address;
    }
};
template <addr Address> class Pergister : Cell<Address> {
    static void f(){
        Cell<Address>::write(111);
    }
};

/*



class DoOnOff {
public:

}
class Unit {

};
*/
