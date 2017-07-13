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
    size_type tree_size = 0;

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

    Node* NewNode(const T & value) {
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
        max = GetByIndex(tree_size - 1);
    }

    void Clear(Node* cur) {
        if (cur != nullptr) {
            Clear(cur->left);
            Clear(cur->right);
            DeleteNode(cur);
        }
    }

    size_type GetIndex(Node* z) {
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

    const Node* GetByIndex(size_type t) const {
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

    ~ my_vector(){
        Clear(root);
    }

    my_vector& operator=(my_vector&& other);

    my_vector& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value);

    void assign(std::initializer_list<T> ilist);

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
        return GetByIndex(0)->key;
    }

    const_reference front() const {
        return GetByIndex(0)->key;
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

    class iterator : public  std::iterator<std::random_access_iterator_tag, T> {
    private:
        Node * ptr;
        const my_vector * tree;

    public:
        iterator() {}
        iterator(Node * p, const my_vector* tree_) : ptr(p), tree(tree_) {}

        const T& operator * () {
            return (ptr->key);
        }

        iterator& operator++() {
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

        iterator operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        iterator& operator--() {
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

        iterator operator--(int) {
            auto old = *this;
            --(*this);
            return old;
        }

        const T* operator -> () {
            return &(ptr->key);
        }

        friend bool operator == (iterator l, iterator r) {
            return l.ptr == r.ptr;
        }

        friend bool operator != (iterator l, iterator r) {
            return !(l == r);
        }
    };

    class const_iterator : public  std::iterator<std::random_access_iterator_tag, T> {
    };

    iterator begin() noexcept {
        return iterator(GetByIndex(0), this);
    }

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept {
        return iterator(tree_end, this);
    }

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    iterator rbegin() noexcept;

    const_iterator rbegin() const noexcept;

    const_iterator crbegin() const noexcept;

    iterator rend() noexcept;

    const_iterator rend() const noexcept;

    const_iterator crend() const noexcept;

    // Capacity

    bool empty() const noexcept {
        return tree_size == 0;
    }

    size_type size() const noexcept {
        return tree_size;
    }

    size_type max_size() const noexcept {
        return tree_size;
    }

    void reserve(size_type new_cap) {}

    size_type capacity() const noexcept {
        return tree_size;
    }

    void shrink_to_fit() {}

    // Modifiers

    void clear() noexcept {
        Clear(root);
    }

    iterator insert(const_iterator pos, const T& value);

    iterator insert(const_iterator pos, T&& value);

    iterator insert(const_iterator pos, size_type count, const T& value);

    template <class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator first, const_iterator last);

    void insert (size_type ind, T value) {
        Insert(ind, NewNode(value));
        Update();
    }

    void erase(size_type ind) {
        Erase(ind);
        Update();
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

    template <class... Args>
    reference emplace_back(Args&&... args);

    void pop_back();

    void resize(size_type count);

    void resize(size_type count, const value_type& value);

    void swap(my_vector& other) noexcept;

    void print() {
        Print(root);
        std::cout << "\n";
    }
};

// Non-member functions

template<class T>
bool operator==(const my_vector<T>& lhs,
                const my_vector<T>& rhs);

template<class T>
bool operator!=(const my_vector<T>& lhs,
                const my_vector<T>& rhs);

template<class T>
bool operator<(const my_vector<T>& lhs,
               const my_vector<T>& rhs);

template<class T>
bool operator<=(const my_vector<T>& lhs,
                const my_vector<T>& rhs);

template<class T>
bool operator>(const my_vector<T>& lhs,
               const my_vector<T>& rhs);

template<class T>
bool operator>=(const my_vector<T>& lhs,
                const my_vector<T>& rhs);

template<class T>
void swap(my_vector<T>& lhs,
          my_vector<T>& rhs);
