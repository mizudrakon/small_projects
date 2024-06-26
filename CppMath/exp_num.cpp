#include "exp_num.h"

template <Arithmetic T>
void exp_num<T>::Reduce() {
    exponent.reduce();
    if (exponent.denomin() == 1){
        base = int(pow(base, exponent.nomin()));
        exponent = 1;
    }
}

template <Arithmetic T>
double exp_num<T>::to_dbl() const {
    return pow(static_cast<double>(base),exponent.to_dbl());
}

template <Arithmetic T>
std::string exp_num<T>::str() const {
    if (exponent == 0) return "1"; //reduce to 1
    std::string text = std::to_string(base); 
    if (exponent == 1) return text;//no need to print exponent
    return text + "^{" + exponent.str() + '}';//also print exponent, if it's nontrivial
}

template <Arithmetic T>
auto exp_num<T>::operator+=(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1)
        base += rhs.base;
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator-=(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1)
        base -= rhs.base;
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator*=(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        base *= rhs.base;
    }
    //same base allows us to add exponents
    else if (base == rhs.base){
        exponent += rhs.exponent;
    }
    Reduce();
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator/=(const exp_num<T>& rhs){
    //exponents = 1 just makes them regular integers
    if (exponent == 1 && rhs.exponent == 1){
        base /= rhs.base;
    }
    else if (base == rhs.base){
        exponent -= rhs.exponent;
    }
    Reduce(); 
    return *this;
} 

template <Arithmetic T>
bool exp_num<T>::add_with(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        *this += rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::sub_with(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        *this -= rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::mult_by(const exp_num<T>& rhs){
    if (base == rhs.base || exponent == 1 && rhs.exponent == 1){
        *this *= rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::div_by(const exp_num<T>& rhs){
    
    if ((base == rhs.GetBase()) || (exponent == 1 && rhs.GetExp() == 1 && base % rhs.GetBase() == 0)){
        *this /= rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
auto operator<=>(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //simple cases:
    if (lhs.GetBase() == rhs.GetBase()) 
        return lhs.GetExp().to_dbl() <=> rhs.GetExp().to_dbl();
    else if (lhs.GetExp() == rhs.GetExp() || 
    ((lhs.GetBase() <=> rhs.GetBase()) == (lhs.GetExp() <=> rhs.GetExp())))
        return static_cast<double>(lhs.GetBase()) <=> static_cast<double>(rhs.GetBase());
    //cases like lhs.base < rhs.base && lhs.exp > rhs.exp:
    return lhs.to_dbl() <=> rhs.to_dbl();
}

template <Arithmetic T>
auto operator<=>(const exp_num<T>& lhs, const T& rhs){
    if (lhs.GetExp() == 1) return lhs.GetBase() <=> rhs;
    return lhs.to_dbl() <=> static_cast<double>(rhs);
}

template <Arithmetic T>
auto operator==(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //some cases that might save time since to_dbl calls pow()
    if (lhs.GetBase() == rhs.GetBase()) return lhs.GetExp() == rhs.GetExp();
    if (lhs.GetExp() == rhs.GetExp()) return lhs.GetBase() == rhs.GetBase();
    if (lhs.GetBase() < rhs.GetBase() && lhs.GetExp() < rhs.GetExp()) return false;
    if (lhs.GetBase() > rhs.GetBase() && lhs.GetExp() > rhs.GetExp()) return false;
    //have to call pow()
    return lhs.to_dbl() == rhs.to_dbl();
}

template <Arithmetic T>
auto operator==(const exp_num<T>& lhs, const T& rhs){
    if (lhs.GetExp() == 1) return lhs.GetBase() == rhs;
    if (lhs.GetExp() >= 1 && lhs.GetBase() > rhs) return false;
    return lhs.to_dbl() == static_cast<double>(rhs);
}

template <Arithmetic T>
exp_num<T> operator+(const exp_num<T>& lhs, const exp_num<T>& rhs){
    if (lhs.GetExp() == 1 && rhs.GetExp() == 1)
        return exp_num<T>(lhs.GetBase()+rhs.GetBase(),1);
    return exp_num<T>{0,-1};//failure
}

template <Arithmetic T>
exp_num<T> operator-(const exp_num<T>& lhs, const exp_num<T>& rhs){
    if (lhs.GetExp() == 1 && rhs.GetExp() == 1)
        return exp_num<T>{lhs.GetBase()-rhs.GetBase(),1};
    return exp_num<T>{0,-1};//failure
}

template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T> operator*(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //exponents = 1 just makes them regular integers
    if (lhs.GetExp() == 1 && rhs.GetExp() == 1){
        return exp_num<T>{lhs.GetBase()*rhs.GetBase(),1};
    }
    //same base allows us to add exponents
    else if (lhs.GetBase() == rhs.GetBase()){
        return exp_num<T>{lhs.GetBase(),lhs.GetExp() + rhs.GetExp()};
    }
    return exp_num<T>{0,-1};//failure
}

template <Arithmetic T>
exp_num<T> operator/(const exp_num<T>& lhs, const exp_num<T>& rhs){
    exp_num<T> answ{0};
    //same base allows us to add exponents
    if (lhs.GetBase() == rhs.GetBase()){
        return exp_num<T>{lhs.GetBase(),lhs.GetExp() - rhs.GetExp()};
    }
    //exponents = 1 just makes them regular integers
    else if (lhs.GetExp() == 1 && rhs.GetExp() == 1){
        return exp_num<T>{lhs.GetBase()/rhs.GetBase(),1};
    }
    return exp_num<T>{0,-1};//failure

}

template <Arithmetic T>
exp_num<T> operator%(const exp_num<T>& lhs, const exp_num<T>& rhs){
    return exp_num<T>{0,-1};//failure
}