#include <initializer_list>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <iostream>
#include <stdexcept>
template <typename T>
class my_vector {
private:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference =  const value_type&;

    static size_type Random(size_type lower, size_type upper) {
        static std::random_device randomDevice;
        static std::mt19937 randomGenerator(randomDevice());
        std::uniform_int_distribution<> randomDistribution(lower, upper);
        return randomDistribution(randomGenerator);
    }

    struct Node {
        Node* p;
        Node* left;
        Node* right;
        T key;
        const size_type prior;
        size_type cnt = 1;
        Node (const T& key_) : p(nullptr), left(nullptr), right(nullptr), key(key_), prior(Random(0, 10000007)) {}
    };

    Node * root = nullptr;
    Node * tree_end = nullptr;
    Node * max = nullptr;
    Node * min = nullptr;
    size_type tree_size = 0;

    Node * TreeMinimum(Node * x) {
        if (x == nullptr)
            return x;
        while (x->left != nullptr) {
            x = x->left;
        }
        return x;
    }

    Node * TreeMaximum(Node * x) {
        if (x == nullptr)
            return x;
        while (x->right != nullptr) {
            x = x->right;
        }
        return x;
    }

    size_type Cnt(Node * tree) {
        return tree ? tree-> cnt : 0;
    }

    const size_type Cnt(Node * tree) const {
        return tree ? tree-> cnt : 0;
    }

    void Push(Node * tree) {
        if (tree) {
        }
    }

    void Update(Node * tree) {
        if (tree) {
            tree->cnt = Cnt(tree->left) + Cnt(tree->right) + 1;
        }
    }

    void Merge(Node* left, Node* right, Node* &t) {
        Push(left);
        Push(right);

        if (!left || !right) {
            t = !left ? right : left;
        } else if (left->prior > right->prior) {
            Merge(left->right, right, left->right);
            t = left;
        } else {
            Merge(left, right->left, right->left);
            t = right;
        }
        if (t->left)
            t->left->p = t;
        if (t->right)
            t->right->p = t;
        Update(t);
    }

    void Split(Node *t, Node *&left, Node *& right, size_type key) {
        Push(t);
        if (!t) {
            left = nullptr;
            right = nullptr;
        } else if (Cnt(t->left) >= key) {
            Split(t->left, left, t->left, key);
            right = t;
        } else {
            Split(t->right, t->right, right, key - Cnt(t->left) - 1);
            left = t;
        }
        if (right != nullptr)
            right->p = nullptr;
        if (left != nullptr)
            left->p = nullptr;
        Update(left);
        Update(right);
    }

    void Print(Node* tree) {
        if (tree) {
            Push(tree);
            Print(tree->left);
            std::cout << tree->key << " ";
            Print(tree->right);
        }
    }

    void Insert(size_type t, Node* node) {
        Node* tmp1 = nullptr;
        Node* tmp2 = nullptr;
        Split(root, tmp1, tmp2, t);
        Merge(tmp1, node, tmp1);
        Merge(tmp1, tmp2, root);
    }

    void Erase(size_type t) {
        Node* tmp1 = nullptr;
        Node* tmp2 = nullptr;
        Node* tmp3 = nullptr;
        Split(root, tmp1, tmp2, t + 1);
        Split(tmp1, tmp1, tmp3, t);
        Merge(tmp1, tmp2, root);
        DeleteNode(tmp3);
    }

    Node* NewNode(const T& value) {
        Node* t = new Node(value);
        ++tree_size;
        return t;
    }

    void DeleteNode(Node * q) {
        if (q == nullptr)
            return;
        --tree_size;
        delete q;
    }

    void Update() {
        max = TreeMaximum(root);
        min = TreeMinimum(root);
    }

    void Clear(Node* cur) {
        if (cur != nullptr) {
            Clear(cur->left);
            Clear(cur->right);
            DeleteNode(cur);
        }
    }

    size_type GetIndex(Node* u) {
        Node* z = u;
        Node* tmp = z;
        size_type index = Cnt(z->left);
        z = z->p;
        while (z != nullptr) {
            if (tmp == z->right)
                index += 1 + Cnt(z->left);
            tmp = z;
            z = z->p;
        }
        return index;
    }

    Node* GetByIndex(size_type t) {
        if (tree_size == 0)
            return tree_end;
        Node* cur = root;
        while (t != Cnt(cur->left)) {
            if (t < Cnt(cur->left))
                cur = cur->left;
            else {
                t -= Cnt(cur->left) + 1;
                cur = cur->right;
            }
        }
        return cur;
    }

    Node* GetByIndex(size_type t) const {
        if (t >= tree_size)
            return tree_end;
        Node* cur = root;
        while (t != Cnt(cur->left)) {
            if (t < Cnt(cur->left))
                cur = cur->left;
            else {
                t -= Cnt(cur->left) + 1;
                cur = cur->right;
            }
        }
        return cur;
    }

public:
    // Member functions

    my_vector(){}

    my_vector(size_type count, const T& value) {
        while (count) {
            count--;
            push_back(value);
        }
    }

    my_vector(size_type count) {
        while (count) {
            count--;
            push_back(T());
        }
    }

    my_vector(const my_vector<T>& other) {
        for (auto i : other) {
            push_back(std::move(i));
        }
    }

    my_vector(my_vector<T>&& other) {
        for (auto i : other) {
            push_back(std::move(i));
        }
    }

    template<typename InputIt>
    my_vector(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    my_vector(std::initializer_list<T> init) : my_vector(init.begin(), init.end()) {}

    ~ my_vector(){
        Clear(root);
    }

    my_vector& operator=(const my_vector& other) {
        my_vector<T> temp(std::move(other));
        temp.swap(*this);
        return *this;
    }

    my_vector& operator=(std::initializer_list<T> ilist) {
        my_vector<T> temp(ilist);
        *this = temp;
        return *this;
    }

    void assign(size_type count, const T& value) {
        clear();
        while (count) {
            push_back(value);
        }
    }

    template< typename InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    void assign(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
    }

    // Element access

    reference at(size_type pos) {
        if (!(pos < tree_size)) {
            //something bad
        } else
            return this[pos];
    }

    const_reference at(size_type pos) const{
        if (!(pos < tree_size)) {
            // something bad
        } else
            return this[pos];
    }

    reference operator[](size_type pos) {
        return GetByIndex(pos)->key;
    }

    const_reference operator[](size_type pos) const {
        return GetByIndex(pos)->key;
    }

    reference front() {
        return min->key;
    }

    const_reference front() const {
        return min->key;
    }

    reference back() {
        return GetByIndex(tree_size - 1)->key;
    }

    const_reference back() const {
        return GetByIndex(tree_size - 1)->key;
    }

    T* data() noexcept {
        return &front();
    }

    const T* data() const noexcept {
        return &front();
    }

    // Iterators

    class const_iterator {
    protected:
        Node * ptr;
        const my_vector * tree;
    public:
        const_iterator() {}
        const_iterator(Node * p, const my_vector* tree_) : ptr(p), tree(tree_) {}

        const T& operator * () const {
            return ptr->key;
        }

        const_iterator& operator++() {
            if (ptr == tree->tree_end)
                return *this;
            if (ptr == tree->max) {
                ptr = tree->tree_end;
                return *this;
            }
            if (ptr->right != nullptr) {
                ptr = ptr->right;
                while (ptr->left != nullptr) {
                    ptr = ptr->left;
                }
            } else {
                while (ptr->p != nullptr && ptr->p->right == ptr) {
                    ptr = ptr->p;
                }
                if (ptr->p != nullptr)
                    ptr = ptr->p;
            }
            return *this;
        }

        const_iterator operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        const_iterator& operator--() {
            if (ptr == tree->min)
                return *this;
            if (ptr == tree->tree_end) {
                ptr = tree->max;
                return *this;
            }
            if (ptr->left != nullptr) {
                ptr = ptr->left;
                while (ptr->right != nullptr) {
                    ptr = ptr->right;
                }
            } else {
                while (ptr->p->left == ptr) {
                    ptr = ptr->p;
                }
                if (ptr->p != nullptr)
                    ptr = ptr->p;
            }
            return *this;
        }

        const_iterator operator--(int) {
            auto old = *this;
            --(*this);
            return old;
        }

        const_iterator& operator+=(int n) {
            if (n >= 0)
                while(n--) ++(*this);
            else
                while(n++) --(*this);
            return *this;
        }

        const_iterator operator+(int n) {
            auto temp = this;
            return *(temp += n);
        }

        const_iterator& operator-=(int n) {
            return (*this += -n);
        }

        friend bool operator == (const_iterator l, const_iterator r) {
            return l.ptr == r.ptr;
        }

        friend bool operator != (const_iterator l, const_iterator r) {
            return !(l == r);
        }
        int operator-(const_iterator z) {
            const_iterator tmp_for = const_iterator(ptr, tree);
            const_iterator tmp_rev = const_iterator(ptr, tree);
            int cnt_for = 0;
            int cnt_rev = 0;
            while (tmp_for != z && tmp_rev != z) {
                cnt_rev++;
                tmp_rev--;
                cnt_for--;
                tmp_for++;
            }
            if (tmp_for == z)
                return cnt_for;
            return cnt_rev;
        }

        reference operator[](int n) {
            const_iterator q = const_iterator(ptr, tree);
            q += n;
            return (q.ptr->key);
        }

        const T* operator -> () {
            return &(ptr->key);
        }

        bool operator < (const_iterator b) {
            return *this - b > 0;
        }

        bool operator > (const_iterator b) {
            return b < *this;
        }

        bool operator >= (const_iterator b) {
            return !(*this < b);
        }

        bool operator <= (const_iterator b) {
            return !(*this > b);
        }

    };

    class iterator : public const_iterator {
    public:
        iterator() : const_iterator() {}
        iterator(Node * p, const my_vector * tree_) : const_iterator(p, tree_) {}

        const T& operator * () {
            return this->ptr->key;
        }

    };

    iterator begin() noexcept {
        return iterator(min, this);
    }

    const_iterator begin() const noexcept {
        return const_iterator(min, this);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(min, this);
    }

    iterator end() noexcept {
        return iterator(tree_end, this);
    }

    const_iterator end() const noexcept {
        return const_iterator(tree_end, this);
    }

    const_iterator cend() const noexcept {
        return const_iterator(tree_end, this);
    }

    // Capacity

    bool empty() const noexcept {
        return tree_size == 0;
    }

    size_type size() const noexcept {
        return tree_size;
    }

    // Modifiers

    void clear() noexcept {
        Clear(root);
        root = nullptr;
        Update();
    }

    iterator insert(const_iterator pos, const T& value) {
        auto t = pos - cbegin();
        Insert(t, NewNode(value));
        Update();
        return iterator(GetByIndex(t), this);
    }

    iterator insert(const_iterator pos, T&& value) {
        auto t = pos - cbegin();
        Insert(t, NewNode(value));
        Update();
        return iterator(GetByIndex(t), this);
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        auto t = pos - cbegin();
        while (count) {
            insert(pos, value);
            count--;
        }
        return iterator(GetByIndex(t), this);
    }

    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        auto t = pos - cbegin();
        while (first != last) {
            insert(pos, *first);
            first++;
        }
        return iterator(GetByIndex(t), this);
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        auto t = pos - cbegin();
        for (auto i : ilist) {
            insert(pos, i);
        }
        return iterator(GetByIndex(t), this);
    }

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        T temp = T(args...);
        return insert(pos, temp);
    }

    iterator erase(const_iterator pos) {
        auto t = pos - cbegin();
        Erase(t);
        Update();
        if (t < size())
            return iterator(GetByIndex(t), this);
        return iterator(tree_end, this);
    }

    iterator erase(const_iterator first, const_iterator last) {
        auto t = first - cbegin();
        auto cnt = last - first;
        while (cnt) {
            cnt--;
            erase(iterator(GetByIndex(t), this));
        }
        if (t < size())
            return iterator(GetByIndex(t), this);
        return iterator(tree_end, this);
    }

    void push_back (T&& value) {
        auto tmp = NewNode(value);
        if (root == nullptr) {
            root = tmp;
        } else {
            Insert(root->cnt, tmp);
        }
        Update();
    }

    void push_back (const T& value) {
        auto tmp = NewNode(value);
        if (root == nullptr) {
            root = tmp;
        } else {
            Insert(root->cnt, tmp);
        }
        Update();
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        push_back(T(args...));
    }
    void pop_back() {
        Erase(size() - 1);
        Update();
    }

    void swap(my_vector<T>& other)  {
        my_vector<T> c;
        for (auto i : other) {
            c.push_back(std::move(i));
        }
        other.clear();
        for (auto i : (*this)) {
            other.push_back(std::move(i));
        }
        clear();
        for (auto i : c) {
            (*this).push_back(std::move(i));
        }
    }

    void resize(size_type count, const value_type& value) {
        while (tree_size > count) {
            pop_back();
        }
        while (count > tree_size) {
            push_back(value);
        }
    }

    void resize(size_type count) {
        resize(count, T());
    }
};

// Non-member functions

template<class T>
bool operator==(const my_vector<T>& lhs,
                const my_vector<T>& rhs) {
    if (lhs.size() == rhs.size()) {
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }
    return false;
}

template<class T>
bool operator!=(const my_vector<T>& lhs,
                const my_vector<T>& rhs) {
    return !(lhs == rhs);
}

template<class T>
bool operator<(const my_vector<T>& lhs,
               const my_vector<T>& rhs) {
    std::size_t i = 0;
    for (; i < lhs.size() && i < rhs.size(); ++i) {
        if (lhs[i] < rhs[i])
            return true;
        if (rhs[i] < lhs[i])
            return false;
    }
    if (lhs.size() >= rhs.size())
        return false;
    return true;
}

template<class T>
bool operator<=(const my_vector<T>& lhs,
                const my_vector<T>& rhs) {
    if (lhs < rhs || lhs == rhs)
        return true;
}

template<class T>
bool operator>(const my_vector<T>& lhs,
               const my_vector<T>& rhs) {
    if (rhs < lhs)
        return true;
    return false;
}
template<class T>
bool operator>=(const my_vector<T>& lhs,
                const my_vector<T>& rhs) {
    if (rhs < lhs || rhs == lhs)
        return true;
    return false;
}

template<class T>
void swap(my_vector<T>& lhs,
          my_vector<T>& rhs) {
    lhs.swap(rhs);
}
