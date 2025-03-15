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
