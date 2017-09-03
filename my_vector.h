#include <initializer_list>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <time.h>


template <typename T>
class my_vector {
private:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference =  const value_type&;
    using difference_type = long;
    static size_type Random(size_type lower, size_type upper) {
        static std::random_device randomDevice;
        static std::mt19937 randomGenerator(randomDevice());
        std::uniform_int_distribution<> randomDistribution(lower, upper);
        return randomDistribution(randomGenerator);
    }

    static const size_type MAX_SIZE = 1e9 + 7;

    struct Node {
        Node* p;
        Node* left;
        Node* right;
        T key;
        const size_type prior;
        size_type cnt = 1;
        Node (const T& key_) : p(nullptr), left(nullptr), right(nullptr), key(key_), prior(Random(0, MAX_SIZE)) {}
    };

    Node * root = nullptr;
    Node * tree_end = nullptr;
    size_type tree_size = 0;

    Node * TreeMinimum(Node * x) const {
        if (tree_size == 0)
            return tree_end;
        while (x->left != nullptr) {
            x = x->left;
        }
        return x;
    }

    size_type Cnt(Node * tree) {
        return tree ? tree-> cnt : 0;
    }

    const size_type Cnt(Node * tree) const {
        return tree ? tree-> cnt : 0;
    }

    void Update(Node * tree) {
        if (tree) {
            tree->cnt = Cnt(tree->left) + Cnt(tree->right) + 1;
            tree->p = nullptr;
            if (tree->left)
                tree->left->p = tree;
            if (tree->right)
                tree->right->p = tree;
        }
    }

    void Merge(Node* left, Node* right, Node* &t) {
        if (!left || !right) {
            t = !left ? right : left;
        } else if (left->prior > right->prior) {
            Merge(left->right, right, left->right);
            t = left;
        } else {
            Merge(left, right->left, right->left);
            t = right;
        }
        Update(t);
    }

    void Split(Node *t, Node *&left, Node *& right, size_type key) {
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
        Update(left);
        Update(right);
    }

    void Print(Node* tree) {
        if (tree) {
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

    void Clear(Node* cur) {
        if (cur != nullptr) {
            Clear(cur->left);
            Clear(cur->right);
            DeleteNode(cur);
        }
    }

    size_type GetIndex(Node* u) const {
        if (u == tree_end) {
            return tree_size;
        }
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
        if (t == 0 && tree_size == 0) {
            return root;
        }
        if (t == tree_size)
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
        if (t == 0 && tree_size == 0) {
            return root;
        }
        if (t == tree_size)
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
        return TreeMinimum(root)->key;
    }

    const_reference front() const {
        return TreeMinimum(root)->key;
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

    class const_iterator : public std::iterator<std::random_access_iterator_tag, T> {
    protected:
        Node * ptr;
        const my_vector * tree;
    public:
        const_iterator() {}
        const_iterator(Node * p, const my_vector* tree_) : ptr(p), tree(tree_) {}

        Node* node() {
            return ptr;
        }

        const_iterator& operator=(const const_iterator& other) {
            ptr = other.ptr;
            return *this;
        }

        const T& operator*() const {
            return ptr->key;
        }

        T* operator -> () const  {
            return &(ptr->key);
        }

        const_iterator& operator++() {
            if (ptr == tree->tree_end)
                return *this;
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
                else
                    ptr = tree->tree_end;
            }
            return *this;
        }

        const_iterator operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        const_iterator& operator--() {
            if (ptr == tree->tree_end) {
                ptr = tree->root;
                while (ptr->right != nullptr) {
                    ptr = ptr->right;
                }
                return *this;
            }
            if (ptr->left != nullptr) {
                ptr = ptr->left;
                while (ptr->right != nullptr) {
                    ptr = ptr->right;
                }
            } else {
                while (ptr->p != nullptr && ptr->p->left == ptr) {
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

        const_reference operator[](difference_type n) const {
            const_iterator q = const_iterator(ptr, tree);
            q += n;
            return (q.ptr->key);
        }

        reference operator[](difference_type n) {
            const_iterator q = const_iterator(ptr, tree);
            q += n;
            return (q.ptr->key);
        }

        const_iterator& operator+=(difference_type n) {
            difference_type temp;
            temp = tree->GetIndex(ptr) + n;
            ptr = tree->GetByIndex(temp);
            return *this;
        }

        const_iterator operator+(difference_type n) {
            auto temp = *this;
            return temp += n;
        }

        const_iterator operator-(difference_type n) {
            const_iterator temp = *this;
            return temp -= n;
        }

        const_iterator& operator-=(difference_type n) {
            return (*this += -n);
        }

        friend bool operator == (const_iterator l, const_iterator r) {
            return l.ptr == r.ptr;
        }

        friend bool operator != (const_iterator l, const_iterator r) {
            return !(l == r);
        }

        difference_type operator-(const_iterator z) {
            difference_type first = tree->GetIndex(z.ptr);
            difference_type second = tree->GetIndex(ptr);
            difference_type result = second - first;
            return result;
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

        friend const_iterator operator+(difference_type n, const const_iterator& it) {
            const_iterator temp = it;
            return temp += n;
        }
    };

    class iterator : public const_iterator {
    public:
        iterator() : const_iterator() {}
        iterator(Node * p, const my_vector * tree_) : const_iterator(p, tree_) {}

        iterator& operator=(const iterator& other) {
            this->ptr = other.ptr;
            return *this;
        }

        T& operator * () {
            return this->ptr->key;
        }

        T* operator -> () const  {
            return &(this->ptr->key);
        }

        iterator& operator++() {
            if (this->ptr->right != nullptr) {
                this->ptr = this->ptr->right;
                while (this->ptr->left != nullptr) {
                    this->ptr = this->ptr->left;
                }
            } else {
                while (this->ptr->p != nullptr && this->ptr->p->right == this->ptr) {
                    this->ptr = this->ptr->p;
                }
                if (this->ptr->p != nullptr)
                    this->ptr = this->ptr->p;
                else
                    this->ptr = this->tree->tree_end;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator& operator--() {
            if (this->ptr == this->tree->tree_end) {
                this->ptr = this->tree->root;
                while (this->ptr->right != nullptr) {
                    this->ptr = this->ptr->right;
                }
                return *this;
            }
            if (this->ptr->left != nullptr) {
                this->ptr = this->ptr->left;
                while (this->ptr->right != nullptr) {
                    this->ptr = this->ptr->right;
                }
            } else {
                while (this->ptr->p != nullptr && this->ptr->p->left == this->ptr) {
                    this->ptr = this->ptr->p;
                }
                if (this->ptr->p != nullptr)
                    this->ptr = this->ptr->p;
            }
            return *this;
        }

        iterator operator--(int) {
            iterator old = *this;
            --(*this);
            return old;
        }

        const_reference operator[](difference_type n) const {
            iterator q = iterator(this->ptr, this->tree);
            q += n;
            return (q.ptr->key);
        }

        reference operator[](difference_type n) {
            iterator q = iterator(this->ptr, this->tree);
            q += n;
            return (q.ptr->key);
        }

        iterator& operator+=(difference_type n) {
            //std::cout << "fuck123 " << this->tree->GetIndex(this->ptr) << "\n";
            difference_type temp;
            temp = this->tree->GetIndex(this->ptr) + n;
            this->ptr = this->tree->GetByIndex(temp);
            return *this;
        }

        iterator operator+(difference_type n) {
            iterator temp = *this;
            return temp += n;
        }

        iterator operator-(difference_type n) {
            iterator temp = *this;
            return temp -= n;
        }

        iterator& operator-=(difference_type n) {
            return (*this += -n);
        }

        friend bool operator == (iterator l, iterator r) {
            return l.ptr == r.ptr;
        }

        friend bool operator != (iterator l, iterator r) {
            return !(l == r);
        }

        difference_type operator-(const iterator z) const {
            difference_type first = this->tree->GetIndex(z.ptr);
            difference_type second = this->tree->GetIndex(this->ptr);
            difference_type result = second - first;
            return result;
        }

        bool operator < (iterator b) {
            return (*this - b) > 0;
        }

        bool operator > (iterator b) {
            return b < *this;
        }

        bool operator >= (iterator b) {
            return !(*this < b);
        }

        bool operator <= (iterator b) {
            return !(*this > b);
        }

        friend iterator operator+(int n, const iterator& it) {
            iterator temp = it;
            return temp += n;
        }
    };


    iterator begin() {
        return iterator(TreeMinimum(root), this);
    }

    const_iterator begin() const{
        return const_iterator(TreeMinimum(root), this);
    }

    const_iterator cbegin() const{
        return const_iterator(TreeMinimum(root), this);
    }

    iterator end() {
        return iterator(tree_end, this);
    }

    const_iterator end() const{
        return const_iterator(tree_end, this);
    }

    const_iterator cend() const {
        return const_iterator(tree_end, this);
    }

    // Capacity

    bool empty() const {
        return tree_size == 0;
    }

    size_type size() const {
        return tree_size;
    }

    // Modifiers

    void clear() noexcept {
        Clear(root);
        root = nullptr;
    }

    iterator insert(const_iterator pos, const T& value) {
        size_type temp = GetIndex(pos.node());
        Insert(temp, NewNode(value));
        return iterator(GetByIndex(temp), this);
    }

    iterator insert(const_iterator pos, T&& value) {
        size_type temp = GetIndex(pos.node());
        Insert(temp, NewNode(value));
        return iterator(GetByIndex(temp), this);
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
        return iterator(GetByIndex(t), this);
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
    }

    void push_back (const T& value) {
        auto tmp = NewNode(value);
        if (root == nullptr) {
            root = tmp;
        } else {
            Insert(root->cnt, tmp);
        }
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        push_back(T(args...));
    }

    void pop_back() {
        Erase(size() - 1);
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
