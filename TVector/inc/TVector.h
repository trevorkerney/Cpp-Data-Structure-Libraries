#pragma once

#include <stdexcept>

enum sortingMethod
{
    quick, insertion, merge, shell
};

template<typename type>
class TVector
{
private:

    type* i_vector;

    unsigned int i_capacity;
    unsigned int i_size;

    float i_growth_multiplier;

    static const unsigned int MAX_CAPACITY = 65535;

    void copy(const TVector& p_other)
    {
        if (p_other.i_capacity > 0)
        {
            if (i_capacity > 0)
            {
                delete[] i_vector;
            }
            i_vector = new type[p_other.i_capacity];

            for (int _i = 0; _i < p_other.i_size; _i++)
            {
                i_vector[_i] = p_other[_i];;
            }
        }
        i_capacity = p_other.i_capacity;
        i_size = p_other.i_size;
        i_growth_multiplier = p_other.i_growth_multiplier;
    }

    int compare(const type& p_obj1, const type& p_obj2) const
    {
        throw std::runtime_error("There is no comparison function provided for this type.");
    }
    int min(const int& p_i1, const int& p_i2) const
    {
        if (compare(i_vector[p_i2], i_vector[p_i1]) < 0)
            return p_i2;
        else
            return p_i1;
    }
    int max(const int& p_i1, const int& p_i2) const
    {
        if (compare(i_vector[p_i1], i_vector[p_i2]) < 0)
            return p_i2;
        else
            return p_i1;
    }

    void swap(const int& p_index1, const int& p_index2)
    {
        type obj = i_vector[p_index1];
        i_vector[p_index1] = i_vector[p_index2];
        i_vector[p_index2] = obj;
    }

    void quick_sort(int p_low, int p_high)
    {
        auto median3 = [&](const int& p_i1, const int& p_i2, const int& p_i3)
        {
            // algorithm provided by Gyorgy Szekely - https://stackoverflow.com/a/19027761
            return max(min(p_i1, p_i2), min(max(p_i1, p_i2), p_i3));
        };
        auto partition = [&](int p_low, int p_high)
        { 
            int pivot_index = median3(p_low, p_high / 2, p_high - 1);
            type pivot = i_vector[pivot_index];
            int _i = p_low - 1;
            int _j = p_high;
            while (_i < _j)
            {
                do
                {
                    _i++;
                } while (compare(i_vector[_i], pivot) <= 0 && _i < p_high);
                do
                {
                    _j--;
                } while (compare(i_vector[_j], pivot) > 0);
                if (_i < _j)
                {
                    if (_j == pivot_index)
                    {
                        pivot_index = _i;
                    }
                    swap(_i, _j);
                }
            }
            swap(pivot_index, _j);
            return _j;
        };

        if (p_low < p_high - 1)
        {
            int pivot = partition(p_low, p_high);
            quick_sort(p_low, pivot);
            quick_sort(pivot + 1, p_high);
        }
    }

public:

    TVector(const unsigned int& p_initial_capacity = 0, const unsigned int& p_growth_multiplier = 2)
    {
        if (p_initial_capacity >= 0 && p_initial_capacity < MAX_CAPACITY)
        {
            i_size = 0;
            i_capacity = 0;
            i_growth_multiplier = p_growth_multiplier;
            if (p_initial_capacity > 0)
            {
                reserve(p_initial_capacity);
            }
        }
        else
        {
            throw std::runtime_error("Out of allowed TArray range: 0 - " + MAX_CAPACITY);
        }
    }
    TVector(const TVector& p_other)
    {
        i_capacity = 0;
        i_size = 0;
        i_growth_multiplier = 2;
        copy(p_other);
    }
    ~TVector()
    {
        if (i_capacity > 0)
        {
            delete[] i_vector;
            i_vector = nullptr;
        }
        i_capacity = 0;
    }

    TVector& operator=(const TVector& p_other)
    {
        if (this != &p_other)
        {
            copy(p_other);
        }    
        return *this;    
    }

    type& at(const int& p_index) const
    {
        if (p_index < 0 || p_index > i_size)
        {
            throw std::runtime_error("Out of range");
        }
        return i_vector[p_index];
    }
    type& operator[](const int& p_index) const
    {
        return at(p_index);
    }

    unsigned int size() const
    {
        return i_size;
    }

    void set_growth_multiplier(const float& p_multiplier)
    {
        i_growth_multiplier = p_multiplier;
    }

    float growth_multiplier() const
    {
        return i_growth_multiplier;
    }

    unsigned int reserve(const int& p_amount)
    {
        unsigned int old_memory_size = i_capacity;

        if ((p_amount == MAX_CAPACITY && i_capacity == MAX_CAPACITY) || p_amount <= i_capacity)
        {
            return 0;
        }
        else if (p_amount <= MAX_CAPACITY)
        {
            type* new_array = new type[p_amount];
            if (i_capacity > 0)
            {
                for (unsigned int _i = 0; _i < i_size; _i++)
                {
                    new_array[_i] = i_vector[_i];
                }
                delete[] i_vector;
            }
            i_vector = new_array;
            i_capacity = p_amount;
            return p_amount - old_memory_size;
        }
        else
        {
            return reserve(MAX_CAPACITY);
        }
    }

    unsigned int shrink()
    {
        int old_memory_size = i_capacity;
        if (i_capacity > i_size)
        {
            type* new_array;
            if (i_size > 0)
            {
                new_array = new type[i_size];
                
                for (unsigned int _i = 0; _i < i_size; _i++)
                {
                    new_array[_i] = i_vector[_i];
                }
                delete[] i_vector;
                i_vector = new_array;
            }
            else
            {
                delete[] i_vector;
                i_vector = nullptr;
            }
            i_capacity = i_size;
            return old_memory_size - i_size;
        }
        else
        {
            return 0;
        }
    }

    void push(const type& p_obj)
    {
        if (i_size < i_capacity)
        {
            i_vector[i_size] = p_obj;
            i_size++;
        }
        else
        {
            if (i_capacity == 0)
            {
                reserve(1);
            }
            else if (i_capacity < MAX_CAPACITY)
            {
                reserve(i_capacity * i_growth_multiplier);
            }
            else
            {
                throw std::runtime_error("Exceeded maximum capacity");
            }
            push(p_obj);
        }
    }

    void push(const type* p_objs, const int& p_size)
    {
        for (int _i = 0; _i < p_size; _i++)
        {
            push(p_objs[_i]);
        }
    }

    type pull()
    {
        if (i_size != 0)
        {
            type& obj = i_vector[--i_size];
            return obj;
        }
        else
        {
            throw std::runtime_error("TVector is empty");
        }
    }

    void insert(const type& p_obj, const int& p_index)
    {
        type obj_swap1;
        type obj_swap2;
        if (i_size < i_capacity)
        {
            obj_swap1 = at(p_index);
            at(p_index) = p_obj;
            i_size++;
        }
        else
        {
            if (i_capacity < MAX_CAPACITY)
            {
                reserve(i_size * i_growth_multiplier);
                obj_swap1 = at(p_index);
                at(p_index) = p_obj;
                i_size++;
            }
            else
            {
                throw std::runtime_error("Exceeded maximum capacity");
            }
        }
        for (int _i = p_index + 1; _i < i_size; _i++)
        {
            obj_swap2 = i_vector[_i];
            i_vector[_i] = obj_swap1;
            obj_swap1 = obj_swap2;
        }
    }

    type erase(const int& p_index)
    {
        type obj;
        if (i_size > 0)
        {
            obj = at(p_index);
        }
        else
        {
            throw std::runtime_error("TVector is empty");
        }
        for (int _i = p_index; _i < i_size - 1; _i++)
        {
            i_vector[_i] = i_vector[_i + 1];
        }
        i_size--;
        return obj;
    }

    void sort(const sortingMethod& p_method = sortingMethod::quick)
    {
        switch(p_method)
        {
        case sortingMethod::quick:
            quick_sort(0, i_size);
            break;
        
        case sortingMethod::insertion:
            //insertion_sort();
            break;
        
        case sortingMethod::merge:
            // merge_sort();
            break;
        
        case sortingMethod::shell:
            // shell_sort();
            break;

        default:
            throw std::runtime_error("Invalid sorting method.");
        }
    }
};

template<>
inline int TVector<int>::compare(const int& p_obj1, const int& p_obj2) const
{
    int comparison = p_obj1 - p_obj2;
    if (comparison != 0)
        if (comparison > 0)
            return 1;
        else
            return -1;
    else
        return 0;
}

template<>
inline int TVector<float>::compare(const float& p_obj1, const float& p_obj2) const
{
    float comparison = p_obj1 - p_obj2;
    if (comparison != 0)
        if (comparison > (float)0)
            return 1;
        else
            return -1;
    else
        return 0;
}

template<>
inline int TVector<double>::compare(const double& p_obj1, const double& p_obj2) const
{
    double comparison = p_obj1 - p_obj2;
    if (comparison != 0)
        if (comparison > (double)0)
            return 1;
        else
            return -1;
    else
        return 0;
}

template<>
inline int TVector<std::string>::compare(const std::string& p_obj1, const std::string& p_obj2) const
{
    int comparison = p_obj1.compare(p_obj2);
    if (comparison != 0)
        if (comparison > 0)
            return 1;
        else
            return -1;
    else
        return 0;
}
