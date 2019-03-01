template <class T> T mempty = delete;
template <class T> T mappend(T, T) = delete;

// concept is keywords since C++20
template <class M> concept bool Monoid = requires (M m) {
    { mempty<M> } -> M;
    { mappend(m, m); } -> m;
};

template<>
std::string mempty<std::string> = {""};

std::string mappend(std::string s1, std::string s2) {
    return s1 + s2;
}
int main() {
    return 0;
}
