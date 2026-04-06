#pragma once

namespace pulvis::tl::tests
{
    int Add(int _a, int _b)
    {
        return _a + _b;
    }

    int Subtract(int _a, int _b)
    {
        return _a - _b;
    }

    int Multiply(int _a, int _b)
    {
        return _a * _b;
    }

    int Divide(int _a, int _b)
    {
        return _a / _b;
    }

    void Sum(int _a, int _b, int& _out)
    {
        _out = _a + _b;
    }

    class TestClass
    {
    public:

        int Add(int _a, int _b)
        {
            return _a + _b;
        }

        int Subtract(int _a, int _b)
        {
            return _a - _b;
        }

        int Multiply(int _a, int _b)
        {
            return _a * _b;
        }

        int Divide(int _a, int _b)
        {
            return _a / _b;
        }

        void Sum(int _a, int _b, int& _out)
        {
            _out = _a + _b;
        }
    };
}
