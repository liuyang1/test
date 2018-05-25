// std.h -- Simple versions of sort, vector, string, pair, map

#ifndef STD_H
#define STD_H

// Swap
template <class T>
inline void swap(T& a, T& b) {
  T tmp=a;
  a=b;
  b=tmp;
}

// Quicksort array or vector of *T with < defined on elements
template <class T>
void sort(T begin, T end) {
  if (begin < end-1) {
    T pivot=begin;
    T i=begin-1;
    T j=end;
    while (true) {
      do --j; while (*pivot < *j);
      do ++i; while (*i < *pivot);
      if (i < j) {
        swap(*i, *j);
        if (pivot==i) pivot=j;
        else if (pivot==j) pivot=i;
      }
      else break;
    }
    ++j;
    sort(begin, j);
    sort(j, end);
  }
}

// Simple array of T, similar to vector<T>.
// The index operator allocates elements as needed.
template <class T> class vector {
private:
  T* p;     // Pointer to allocated elements
  int n;    // Number of elements allocated (capacity)
  int siz;  // Number of elements in use (size), siz <= n
public:
  typedef T* iterator;                    // Type of iterator (random)
  typedef const T* const_iterator;        // Type of const iterator
  int size() const {return siz;}          // Number of elements
  int capacity() const {return n;}        // Max size until reallocation
  void reserve(int i);                    // Set capacity
  iterator begin() {return p;}            // Iterator to first element
  const iterator begin() const {return p;}
  iterator end() {return p+size();}       // Iterator to last+1 element
  const iterator end() const {return p+size();}
  vector(): p(0), n(0), siz(0) {}         // Constructor
  ~vector() {if (p) delete[] p;}          // Destructor         
  vector(const vector<T>&);               // Copy constructor
  vector<T>& operator=(const vector<T>&); // Assignment
  T& operator[](int);                     // Index, allocates memory
  const T& operator[](int) const;         // Const index, default elem.
  void push_back(const T& x);             // Append element
  void pop_back() {if (siz>0) --siz;}     // Remove element (not destroyed)
  T& back() {return (*this)[size()-1];}   // Last element
  const T& back() const {return (*this)[size()-1];}
};

// Set capacity (n) to at least i by allocating new elements
template <class T>
void vector<T>::reserve(int i) {
  if (i>n) {
    T* q=new T[i];
    if (p) {
      for (int j=0; j<n; ++j)
        q[j]=p[j];
      delete[] p;
    }
    p=q;
    n=i;
  }
}

// Copy constructor, do deep copy, trimming unused elements
template <class T>
vector<T>::vector(const vector<T>& a): n(0), p(0), siz(0) {
  for (int i=a.size()-1; i>=0; --i)
    (*this)[i]=a[i];
}

// Assignment, do deep copy, trimming unused elements
template <class T>
vector<T>& vector<T>::operator=(const vector<T>& a) {
  if (&a!=this) {
    if (p)
      delete [] p;
    p=0;
    n=siz=0;
    for (int i=a.size()-1; i>=0; --i)
      (*this)[i]=a[i];
  }
  return *this;
}

// Safe index, increase size and capacity if out of bounds
template <class T>
T& vector<T>::operator[](int i) {
  if (i<0)
    i=0;
  reserve(i+1);
  if (i>=siz)
    siz=i+1;
  return p[i];
}

// Safe index for const vectors, return a default T if out of bounds
template <class T>
const T& vector<T>::operator[](int i) const {
  const static T default_element = T();
  return i>=0 && i<size() ? p[i] : default_element;
}

// Push element on end, optimize for speed
template <class T>
void vector<T>::push_back(const T& x) {
  if (size()>=capacity())
    reserve(size()*2);
  (*this)[size()]=x;
}

///////////// A string is a vector<char> with a hidden 0 on the end

class string: public vector<char> {
public:
  string(const char* s=0, int n=-1);  // Create, size=n
  const char* c_str() const {return &(*this)[0];}  // Convert to char*
  string(const string& s, int m=0, int n=-1);  // Copy, substring
  string substr(int m=0, int n=-1) {return string(*this, m, n);}
  string& operator=(const string& s);  // Assign, trim
  void push_back(char c);  // Overrides vector<char>, appends hidden NUL
  void pop_back() {back()=0; vector<char>::pop_back();}
  string(char c): vector<char>() {push_back(c);}
};

// Create, max length n
string::string(const char* s, int n) {
  if (s && n)
    while (*s && n--)
      push_back(*s++);
}

// Copy s[m...m+n-1] or s[m...] or s
string::string(const string& s, int m, int n) {
  if (m<0) m=0;
  if (m>s.size()) m=s.size();
  if (n<0 || n>s.size()-m) n=s.size()-m;
  if (n>0) {
    (*this)[n]=0;
    for (int i=0; i<n; ++i)
      (*this)[i]=s.c_str()[m+i];
    vector<char>::pop_back();
  }
}

// Assign
string& string::operator=(const string& s) {
  if (&s!=this) {
    if (s.size()<=size())
      vector<char>::operator=(string());  // Shrink
    for (int i=s.size(); i>=0; --i)
      (*this)[i]=s[i];
    vector<char>::pop_back();
  }
  return *this;
}

// Push_back with hidden 0 appended
inline void string::push_back(char c) {
  vector<char>::push_back(c);
  vector<char>::push_back(0);
  vector<char>::pop_back();
}

// string comparison
int scmp(const string& s1, const string& s2) {
  for (int i=0; i<s1.size() && i<s2.size(); ++i)
    if (s1[i]!=s2[i])
      return s1[i]-s2[i];
  return s1.size()-s2.size();
}

int scmp(const char* s1, const char* s2) {  // Like strcmp
  while (*s1 && *s1 == *s2) {
    ++s1;
    ++s2;
  }
  return *s1-*s2;
}
#define op(x)                                                 \
inline bool operator x (const string& s1, const string& s2) { \
  return scmp(s1, s2) x 0;}                                   \
inline bool operator x (const string& s1, const char* s2) {   \
  return scmp(s1.c_str(), s2) x 0;}                           \
inline bool operator x (const char* s1, const string& s2) {   \
  return scmp(s1, s2.c_str()) x 0;}
op(==)
op(!=)
op(<)
op(<=)
op(>)
op(>=)
#undef op

// Concatenation
string& operator+=(string& a, const string& b) {
  for (string::const_iterator p=b.begin(); p!=b.end(); ++p)
    a.push_back(*p);
  return a;
}

string& operator+=(string& a, char c) {
  a.push_back(c);
  return a;
}

string& operator+=(string& a, const char* s) {
  while (*s)
    a.push_back(*s++);
  return a;
}

template <class T>
string operator+(const string& a, const T& b) {
  string result=a;
  result+=b;
  return result;
}

string operator+(char a, const string& b) {return string(a)+b;}
string operator+(const char* a, const string& b) {return string(a)+b;}

#if defined(__IOSTREAM_H) || defined(IOSTREAM_H)

// Output
ostream& operator<<(ostream& o, const string& s) {
  return o << s.c_str();
}

// Read line of input
istream& operator>>(istream& i, string& s) {
  s="";
  char c;
  while (i.get(c) && c!='\n')
    s+=c;
  return i;
}

#endif  // __IOSTREAM_H

///////////// A map is a tree of key-value pairs, accessed through map[key]

template <class T1, class T2>
struct pair {
  T1 first;
  T2 second;
  pair(const T1& a, const T2& b): first(a), second(b) {}
  pair(): first(T1()), second(T2()) {}
};

template <class K, class V>
class Map_iterator;

template <class K, class V>
class map {
  friend class Map_iterator<K,V>;
public:
  class Node: public pair<K,V> {  // Binary tree implementation
    friend class map<K,V>;
    friend class Map_iterator<K,V>;
  private:
    Node *left, *right, *parent;
    bool rparent;  // true if parent->second > second (value)
    Node(const K& k, Node* p, bool rp): pair<K,V>(k, V()),
      left(0), right(0), parent(p), rparent(rp) {}
  };
private:
  Node* root;
  int siz;  // Size
public:
  map(): root(0), siz(0) {}
  int size() const {return siz;}
  V& operator[](const K& key);  // Create node if key not in tree
  const V& operator[](const K& key) const;  // Default value if not in tree
  typedef Map_iterator<K,V> iterator;
  iterator begin() const;
  iterator end() const;
  iterator find(const K& k) const;
};

template <class K, class V>
V& map<K,V>::operator[](const K& key) {
  if (!root) {
    ++siz;
    root=new Node(key, 0, false);
    return root->second;
  }
  for (Node* p=root;;) {
    if (key<p->first) {
      if (!p->left) {
        ++siz;
        p->left=new Node(key, p, true);
        return p->left->second;
      }
      p=p->left;
    }
    else if (p->first<key) {
      if (!p->right) {
        ++siz;
        p->right=new Node(key, p, false);
        return p->right->second;
      }
      p=p->right;
    }
    else
      return p->second;
  }
}

template <class K, class V>
const V& map<K,V>::operator[](const K& key) const {
  static const V empty=V();
  if (!root)
    return empty;
  for (Node* p=root;;) {
    if (key<p->first) {
      if (!p->left)
        return empty;
      p=p->left;
    }
    else if (p->first<key) {
      if (!p->right)
        return empty;
      p=p->right;
    }
    else
      return p->second;
  }
}

template <class K, class V>
class Map_iterator {
private:
  const map<K,V>::Node* p;  // 0 = end
public:
  Map_iterator(const map<K,V>& m);  // Begin of map m
  Map_iterator(): p(0) {}  // End
  Map_iterator(map<K,V>::Node* q): p(q) {}  // Middle
  Map_iterator& operator++();  // Pre-increment only
  bool operator==(const Map_iterator& i) const {return p==i.p;}
  bool operator!=(const Map_iterator& i) const {return !(*this==i);}
  const pair<K,V>& operator*() const {return *p;}
  const pair<K,V>* operator->() const {return p;}
};

template <class K, class V>
Map_iterator<K,V>::Map_iterator(const map<K,V>& m): p(0) {
  if (m.root)  // Find beginning of m
    for (p=m.root; p->left; p=p->left)
      ;
}

// Find next larger p, 0 at end
template <class K, class V>
Map_iterator<K,V>& Map_iterator<K,V>::operator++() {
  if (p) {
    if (p->right) {
      p=p->right;
      while (p->left)
        p=p->left;
    }
    else {
      while (p->parent && !p->rparent)
        p=p->parent;
      p=p->parent;
    }
  }
  return *this;
}

template<class K, class V>
Map_iterator<K,V> map<K,V>::begin() const {return Map_iterator<K,V>(*this);}

template<class K, class V>
Map_iterator<K,V> map<K,V>::end() const {return Map_iterator<K,V>();}

template <class K, class V>
Map_iterator<K,V> map<K,V>::find(const K& key) const {
  if (!root)
    return end();
  for (Node* p=root;;) {
    if (key<p->first) {
      if (!p->left)
        return end();
      p=p->left;
    }
    else if (p->first<key) {
      if (!p->right)
        return end();
      p=p->right;
    }
    else
      return Map_iterator<K,V>(p);
  }
}

#endif  // STD_H

