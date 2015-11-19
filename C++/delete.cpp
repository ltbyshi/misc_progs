int main()
{
    int* pi = new int[10];
    pi[10000] = 123;
    delete[] pi;
    delete[] pi;

    return 0;
}
