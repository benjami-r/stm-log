#include <stdint.h>


class C {
	int a = 5;
public:
	int aGet() {return a;}
};
extern "C" {
	int fcpp(){
		//C c;
		C *c = new C;
		//return c.aGet();
		return c->aGet();
	}
}

template<uint32_t Addr>
class Register {
public:
    static void write(uint32_t value) {
        *reinterpret_cast<volatile uint32_t*>(Addr) = value;
    }

    static uint32_t read() {
        return *reinterpret_cast<volatile uint32_t*>(Addr);
    }
};

// Использование:
using GPIOA_MODER = Register<0x48000000>;  // Адрес регистра MODER для GPIOA

void init() {
	GPIOA_MODER::write(0x55555555);  // Настройка GPIOA
}


//---------------------------------------------------
template <typename Derived>
class Handler {
public:
    void handle() {
        static_cast<Derived*>(this)->onEvent();
    }
};

class ButtonHandler : public Handler<ButtonHandler> {
public:
    void onEvent() {
        // Реакция на нажатие кнопки
    }
};
void init1() {
	// Использование:
	ButtonHandler btn;
	btn.handle();  // Компилятор подставит прямой вызов onEvent()
}



