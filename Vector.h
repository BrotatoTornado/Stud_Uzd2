/**
 * @file Vector.h
 * @brief Pilnavertė std::vector alternatyva – Vector<T, Allocator>
 *
 * Apima visus Member types, Member functions ir Non-member functions.
 *
 * @author v3.0
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

/**
 * @brief Dinaminio masyvo konteineris – std::vector analogas.
 *
 * @tparam T        Saugomų elementų tipas.
 * @tparam Allocator Atminties paskirstytojas (numatytasis: std::allocator<T>).
 */
template <typename T, typename Allocator = std::allocator<T>>
class Vector
{
public:
    //Member types
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using AllocTraits = std::allocator_traits<Allocator>;

    Allocator alloc_;
    pointer data_;
    size_type size_;
    size_type cap_;

    //Vidiniai pagalbiniai metodai

    //Sunaikina visus elementus
    void destroyAll() noexcept
    {
        for (size_type i = 0; i < size_; ++i)
        {
            AllocTraits::destroy(alloc_, data_ + i);
        }
    }

    //Atpalaiduoja paskirtą atminties bloką
    void deallocate() noexcept
    {
        if (data_)
        {
            AllocTraits::deallocate(alloc_, data_, cap_);
            data_ = nullptr;
        }
    }

    //Perskirsto atmintį į naują bloką (move semantika)
    void reallocate(size_type newCap)
    {
        pointer newData = AllocTraits::allocate(alloc_, newCap);
        size_type i = 0;
        try
        {
            for (; i < size_; ++i)
                AllocTraits::construct(alloc_, newData + i,
                                       std::move_if_noexcept(data_[i]));
        }
        catch (...)
        {
            for (size_type j = 0; j < i; ++j)
                AllocTraits::destroy(alloc_, newData + j);
            AllocTraits::deallocate(alloc_, newData, newCap);
            throw;
        }
        destroyAll();
        deallocate();
        data_ = newData;
        cap_  = newCap;
    }

    //Eksponentinis augimas (×2)
    size_type growCap() const noexcept
    {
        return cap_ == 0 ? 1 : cap_ * 2;
    }

public:
    //Konstruktoriai

    /**
     * @brief Numatytasis konstruktorius – sukuria tuščią vektorių.
     */
    Vector() noexcept(noexcept(Allocator())) : alloc_(), data_(nullptr), size_(0), cap_(0) {}

    /**
     * @brief Konstruktorius su paskirstytoju.
     */
    explicit Vector(const Allocator& alloc) noexcept : alloc_(alloc), data_(nullptr), size_(0), cap_(0) {}

    /**
     * @brief Konstruktorius su skaičiumi ir reikšme.
     * @param count Elementų skaičius.
     * @param value Pradinė reikšmė.
     */
    Vector(size_type count, const T& value, const Allocator& alloc = Allocator()) : alloc_(alloc), data_(nullptr), size_(0), cap_(0)
    {
        if (count > 0)
        {
            data_ = AllocTraits::allocate(alloc_, count);
            cap_  = count;
            for (size_type i = 0; i < count; ++i)
            {
                AllocTraits::construct(alloc_, data_ + i, value);
                ++size_;
            }
        }
    }

    /**
     * @brief Konstruktorius su skaičiumi (value-initialized).
     * @param count Elementų skaičius.
     */
    explicit Vector(size_type count, const Allocator& alloc = Allocator()) : Vector(count, T(), alloc) {}

    /**
     * @brief Diapazoninis konstruktorius (InputIterator pora).
     */
    template <typename InputIt, typename = std::enable_if_t<std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
    Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : alloc_(alloc), data_(nullptr), size_(0), cap_(0)
    {
        for (auto it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    /**
     * @brief Kopijavimo konstruktorius.
     */
    Vector(const Vector& other) : alloc_(AllocTraits::select_on_container_copy_construction(other.alloc_)), data_(nullptr), size_(0), cap_(0)
    {
        if (other.size_ > 0)
        {
            data_ = AllocTraits::allocate(alloc_, other.size_);
            cap_ = other.size_;
            for (size_type i = 0; i < other.size_; ++i)
            {
                AllocTraits::construct(alloc_, data_ + i, other.data_[i]);
                ++size_;
            }
        }
    }

    /**
     * @brief Kopijavimo konstruktorius su paskirstytoju.
     */
    Vector(const Vector& other, const Allocator& alloc) : alloc_(alloc), data_(nullptr), size_(0), cap_(0)
    {
        if (other.size_ > 0)
        {
            data_ = AllocTraits::allocate(alloc_, other.size_);
            cap_ = other.size_;
            for (size_type i = 0; i < other.size_; ++i)
            {
                AllocTraits::construct(alloc_, data_ + i, other.data_[i]);
                ++size_;
            }
        }
    }

    /**
     * @brief Perkėlimo konstruktorius.
     */
    Vector(Vector&& other) noexcept : alloc_(std::move(other.alloc_)), data_(other.data_), size_(other.size_), cap_(other.cap_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
    }

    /**
     * @brief Perkėlimo konstruktorius su paskirstytoju.
     */
    Vector(Vector&& other, const Allocator& alloc) noexcept : alloc_(alloc), data_(other.data_), size_(other.size_), cap_(other.cap_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
    }

    /**
     * @brief Inicializatorių sąrašo konstruktorius.
     */
    Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : alloc_(alloc), data_(nullptr), size_(0), cap_(0)
    {
        reserve(init.size());
        for (const auto& v : init)
        {
            push_back(v);
        }
    }

    //Destruktorius

    /**
     * @brief Destruktorius – atlaisvina visus resursus.
     */
    ~Vector()
    {
        destroyAll();
        deallocate();
    }

    //Priskyrimo operatoriai

    /**
     * @brief Kopijavimo priskyrimo operatorius.
     */
    Vector& operator=(const Vector& other)
    {
        if (this == &other)
        {
            return *this;
        }
        clear();
        if constexpr (AllocTraits::propagate_on_container_copy_assignment::value)
        {
            if (alloc_ != other.alloc_)
            {
                deallocate();
                cap_ = 0;
            }
            alloc_ = other.alloc_;
        }
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; ++i)
        {
            AllocTraits::construct(alloc_, data_ + i, other.data_[i]);
            ++size_;
        }
        return *this;
    }

    /**
     * @brief Perkėlimo priskyrimo operatorius.
     */
    Vector& operator=(Vector&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        destroyAll();
        deallocate();
        if constexpr (AllocTraits::propagate_on_container_move_assignment::value)
        alloc_ = std::move(other.alloc_);
        data_ = other.data_;
        size_ = other.size_;
        cap_ = other.cap_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_  = 0;
        return *this;
    }

    /**
     * @brief Inicializatorių sąrašo priskyrimo operatorius.
     */
    Vector& operator=(std::initializer_list<T> init)
    {
        clear();
        reserve(init.size());
        for (const auto& v : init)
        {
            push_back(v);
        }
        return *this;
    }

    // assign

    /**
     * @brief Priskiria count kopijų value reikšmės.
     */
    void assign(size_type count, const T& value)
    {
        clear();
        reserve(count);
        for (size_type i = 0; i < count; ++i)
        {
            push_back(value);
        }
    }

    /**
     * @brief Priskiria diapazoną [first, last).
     */
    template <typename InputIt, typename = std::enable_if_t<std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
    void assign(InputIt first, InputIt last)
    {
        clear();
        for (auto it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    /**
     * @brief Priskiria inicializatorių sąrašą.
     */
    void assign(std::initializer_list<T> init)
    {
        clear();
        reserve(init.size());
        for (const auto& v : init)
        {
            push_back(v);
        }
    }

    // get_allocator

    /**
     * @brief Grąžina naudojamą paskirstytoją.
     */
    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    // -----------------------------------------------------------------------
    // Elementų prieiga
    // -----------------------------------------------------------------------

    /**
     * @brief Prieiga su ribų tikrinimo (meta std::out_of_range jei pos >= size).
     */
    reference at(size_type pos)
    {
        if (pos >= size_)
        {
            throw std::out_of_range("Vector::at – indeksas uz ribu: " + std::to_string(pos));
        }
        return data_[pos];
    }

    /**
     * @brief Const prieiga su ribų tikrinimo.
     */
    const_reference at(size_type pos) const
    {
        if (pos >= size_)
        {
            throw std::out_of_range("Vector::at – indeksas uz ribu: " + std::to_string(pos));
        }
        return data_[pos];
    }

    /// @brief Indeksavimo operatorius (be ribų tikrinimo).
    reference operator[](size_type pos) { return data_[pos]; }
    /// @brief Const indeksavimo operatorius.
    const_reference operator[](size_type pos) const { return data_[pos]; }

    /// @brief Pirmas elementas.
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    /// @brief Paskutinis elementas.
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    /// @brief Rodyklė į vidinį masyvą.
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    //Iteratoriai

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end()  noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator rbegin()  noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend()  noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend()  const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    //Talpa

    /// @brief Ar vektorius tuščias?
    bool empty() const noexcept { return size_ == 0; }
    /// @brief Elementų skaičius.
    size_type size() const noexcept { return size_; }
    /// @brief Maksimalus galimas dydis.
    size_type max_size() const noexcept { return AllocTraits::max_size(alloc_); }
    /// @brief Dabartinė talpa.
    size_type capacity() const noexcept { return cap_; }

    /**
     * @brief Rezervuoja atmintį bent newCap elementams (nekeičia size).
     */
    void reserve(size_type newCap)
    {
        if (newCap <= cap_)
        {
            return;
        }
        reallocate(newCap);
    }

    /**
     * @brief Sumažina talpos perteklių (cap → size).
     */
    void shrink_to_fit()
    {
        if (size_ == cap_)
        {
            return;
        }
        if (size_ == 0)
        {
            deallocate();
            cap_ = 0;
            return;
        }
        reallocate(size_);
    }

    //Modifikatoriai

    /**
     * @brief Pašalina visus elementus (talpa nesikeičia).
     */
    void clear() noexcept
    {
        destroyAll();
        size_ = 0;
    }

    // --- insert ---

    /**
     * @brief Įterpia vieną kopiją prieš pos.
     * @return Iteratorius į įterptą elementą.
     */
    iterator insert(const_iterator pos, const T& value)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        ensureSpace(1);
        shiftRight(idx, 1);
        AllocTraits::construct(alloc_, data_ + idx, value);
        size_++;
        return data_ + idx;
    }

    /**
     * @brief Įterpia vieną elementą (move) prieš pos.
     */
    iterator insert(const_iterator pos, T&& value)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        ensureSpace(1);
        shiftRight(idx, 1);
        AllocTraits::construct(alloc_, data_ + idx, std::move(value));
        ++size_;
        return data_ + idx;
    }

    /**
     * @brief Įterpia count kopijų value prieš pos.
     */
    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        if (count == 0)
        {
            return data_ + idx;
        }
        ensureSpace(count);
        const size_type oldSize = size_;
        shiftRight(idx, count);
        for (size_type i = 0; i < count; ++i)
        {
            const size_type dst = idx + i;
            if (dst < oldSize)
            {
                data_[dst] = value;
            }
            else
            {
                AllocTraits::construct(alloc_, data_ + dst, value);
            }
        }
        size_ += count;
        return data_ + idx;
    }

    /**
     * @brief Įterpia diapazoną [first, last) prieš pos.
     */
    template <typename InputIt, typename = std::enable_if_t<std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        Vector tmp(first, last, alloc_);
        const size_type count = tmp.size_;
        if (count == 0)
        {
            return data_ + idx;
        }
        ensureSpace(count);
        const size_type oldSize = size_;
        shiftRight(idx, count);
        for (size_type i = 0; i < count; ++i)
        {
            const size_type dst = idx + i;
            if (dst < oldSize)
            {
                data_[dst] = std::move(tmp.data_[i]);
            }
            else
            {
                AllocTraits::construct(alloc_, data_ + dst, std::move(tmp.data_[i]));
            }
        }
        size_ += count;
        return data_ + idx;
    }

    /**
     * @brief Įterpia inicializatorių sąrašą prieš pos.
     */
    iterator insert(const_iterator pos, std::initializer_list<T> init)
    {
        return insert(pos, init.begin(), init.end());
    }

    // --- emplace ---

    /**
     * @brief Sukuria elementą vietoje prieš pos.
     */
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        ensureSpace(1);
        shiftRight(idx, 1);
        AllocTraits::construct(alloc_, data_ + idx, std::forward<Args>(args)...);
        size_++;
        return data_ + idx;
    }

    // --- erase ---

    /**
     * @brief Pašalina elementą pos pozicijoje.
     * @return Iteratorius po pašalinto elemento.
     */
    iterator erase(const_iterator pos)
    {
        const size_type idx = static_cast<size_type>(pos - data_);
        for (size_type i = idx; i + 1 < size_; ++i)
        {
            data_[i] = std::move(data_[i + 1]);
        }
        AllocTraits::destroy(alloc_, data_ + size_ - 1);
        size_--;
        return data_ + idx;
    }

    /**
     * @brief Pašalina diapazoną [first, last).
     * @return Iteratorius į pirmą elementą po pašalintų.
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        const size_type idxFirst = static_cast<size_type>(first - data_);
        const size_type idxLast = static_cast<size_type>(last  - data_);
        const size_type count = idxLast - idxFirst;
        if (count == 0)
        {
            return data_ + idxFirst;
        }
        // Perkeliame elementus kairėn
        for (size_type i = idxFirst; i + count < size_; ++i)
        {
            data_[i] = std::move(data_[i + count]);
        }
        // Sunaikinami galo elementai
        for (size_type i = size_ - count; i < size_; ++i)
        {
            AllocTraits::destroy(alloc_, data_ + i);
        }
        size_ -= count;
        return data_ + idxFirst;
    }

    // --- push_back / emplace_back / pop_back ---

    /**
     * @brief Prideda kopiją į galą.
     */
    void push_back(const T& value)
    {
        if (size_ == cap_)
        {
            reallocate(growCap());
        }
        AllocTraits::construct(alloc_, data_ + size_, value);
        size_++;
    }

    /**
     * @brief Prideda (move) elementą į galą.
     */
    void push_back(T&& value)
    {
        if (size_ == cap_)
        {
            reallocate(growCap());
        }
        AllocTraits::construct(alloc_, data_ + size_, std::move(value));
        size_++;
    }

    /**
     * @brief Sukuria elementą vietoje gale.
     * @return Nuoroda į sukurtą elementą.
     */
    template <typename... Args>
    reference emplace_back(Args&&... args)
    {
        if (size_ == cap_)
        {
            reallocate(growCap());
        }
        AllocTraits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        size_++;
        return data_[size_ - 1];
    }

    /**
     * @brief Pašalina paskutinį elementą.
     */
    void pop_back()
    {
        assert(size_ > 0 && "pop_back() iškviesta ant tuščio vektoriaus");
        AllocTraits::destroy(alloc_, data_ + size_ - 1);
        size_--;
    }

    // --- resize ---

    /**
     * @brief Pakeičia dydį į count (nauji elementai – value-initialized).
     */
    void resize(size_type count)
    {
        if (count < size_)
        {
            for (size_type i = count; i < size_; ++i)
            {
                AllocTraits::destroy(alloc_, data_ + i);
            }
            size_ = count;
        }
        else if (count > size_)
        {
            reserve(count);
            for (size_type i = size_; i < count; ++i)
            {
                AllocTraits::construct(alloc_, data_ + i);
                ++size_;
            }
        }
    }

    /**
     * @brief Pakeičia dydį į count, naujus elementus užpildo value.
     */
    void resize(size_type count, const T& value)
    {
        if (count < size_)
        {
            for (size_type i = count; i < size_; ++i)
            {
                AllocTraits::destroy(alloc_, data_ + i);
            }
            size_ = count;
        }
        else if (count > size_)
        {
            reserve(count);
            for (size_type i = size_; i < count; ++i)
            {
                AllocTraits::construct(alloc_, data_ + i, value);
                size_++;
            }
        }
    }

    // --- swap ---

    /**
     * @brief Sukeičia du vektorius vietomis (O(1)).
     */
    void swap(Vector& other) noexcept
    {
        using std::swap;
        swap(alloc_, other.alloc_);
        swap(data_,  other.data_);
        swap(size_,  other.size_);
        swap(cap_,   other.cap_);
    }

private:
    // Vidiniai pagalbiniai metodai insert logikai

    //Garantuoja vietos count naujiems elementams (perskirsto jei reikia)
    void ensureSpace(size_type count)
    {
        if (size_ + count > cap_)
        {
            reallocate(std::max(size_ + count, growCap()));
        }
    }

    /**
     * @brief Pastumia elementus iš [idx, size_) dešinėn per count pozicijų.
     *
     * Elementai į nepatalpintą (uninitialized) sritį – construct,
     * į jau egzistuojančią – move-assign.
     * Rezultate pozicijos [idx, idx+count) – „inicializuotos, bet perkeltos"
     * arba neinicializuotos – tvarkomos insert() metodo.
     *
     * @pre size_ + count <= cap_  (ensureSpace jau iškviesta)
     */
    void shiftRight(size_type idx, size_type count)
    {
        // Perkeliame dešinėn (iš dešinės į kairę, kad neverstume)
        size_type i = size_ + count;
        while (i-->idx + count)
        {
            const size_type src = i - count;
            if (i >= size_)
            {
                AllocTraits::construct(alloc_, data_ + i, std::move(data_[src]));
            }
            else
            {
                data_[i] = std::move(data_[src]);
            }
        }
    }
};

//Non-member palyginimo operatoriai

/// @relates Vector
template <typename T, typename A>
bool operator==(const Vector<T, A>& l, const Vector<T, A>& r)
{
    if (l.size() != r.size())
    {
        return false;
    }
    for (typename Vector<T, A>::size_type i = 0; i < l.size(); ++i)
    {
        if (!(l[i] == r[i]))
        {
            return false;
        }
    }
    return true;
}

/// @relates Vector
template <typename T, typename A>
bool operator!=(const Vector<T, A>& l, const Vector<T, A>& r) { return !(l == r); }

/// @relates Vector
template <typename T, typename A>
bool operator<(const Vector<T, A>& l, const Vector<T, A>& r) { return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()); }

/// @relates Vector
template <typename T, typename A>
bool operator<=(const Vector<T, A>& l, const Vector<T, A>& r) { return !(r < l); }

/// @relates Vector
template <typename T, typename A>
bool operator>(const Vector<T, A>& l, const Vector<T, A>& r) { return r < l; }

/// @relates Vector
template <typename T, typename A>
bool operator>=(const Vector<T, A>& l, const Vector<T, A>& r) { return !(l < r); }

//Non-member swap

/**
 * @brief ADL-friendly swap.
 * @relates Vector
 */
template <typename T, typename A>
void swap(Vector<T, A>& l, Vector<T, A>& r) noexcept { l.swap(r); }

//Non-member erase / erase_if

/**
 * @brief Pašalina visus elementus, lygius value.
 * @return Pašalintų elementų skaičius.
 * @relates Vector
 */
template <typename T, typename A, typename U>
typename Vector<T, A>::size_type erase(Vector<T, A>& c, const U& value)
{
    auto it = std::remove(c.begin(), c.end(), value);
    const auto n = static_cast<typename Vector<T, A>::size_type>(std::distance(it, c.end()));
    c.erase(it, c.end());
    return n;
}

/**
 * @brief Pašalina visus elementus, tenkinančius pred.
 * @return Pašalintų elementų skaičius.
 * @relates Vector
 */
template <typename T, typename A, typename Pred>
typename Vector<T, A>::size_type erase_if(Vector<T, A>& c, Pred pred)
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    const auto n = static_cast<typename Vector<T, A>::size_type>(std::distance(it, c.end()));
    c.erase(it, c.end());
    return n;
}