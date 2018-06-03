using CourseWork.ThreadControllers;

namespace CourseWork
{
    class Sorter<T> where T : System.IComparable<T>
    {
        private IThreadController<T> controller;
        public Sorter(IThreadController<T> controller)
        {
            this.controller = controller;
        }
        private void merge(T [] arr, int  left, int mid, int right)
        {
            int it1 = 0, it2 = 0;
            T[] result = new T[right - left];
            while ((left + it1 < mid) && (mid + it2 < right))
            {
                if (arr[left + it1].CompareTo(arr[mid + it2]) < 0)
                {
                    result[it1 + it2] = arr[left + it1];
                    it1 += 1;
                }
                else
                {
                    result[it1 + it2] = arr[mid + it2];
                    it2 += 1;
                }
            }
            while (left + it1 < mid)
            {
                result[it1 + it2] = arr[left + it1];
                it1 += 1;
            }
            while (mid + it2 < right)
            {
                result[it1 + it2] = arr[mid + it2];
                it2 += 1;
            }
            for (int i = 0; i < it1 + it2; i++)
                arr[left + i] = result[i];
        }

        public void SortMerge(T[] arr)
        {
            int n = arr.Length;
            int itterations = 0;
            for (int i = 1; i < n; i *= 2)
            {
                for (int j = 0; j < n - i; j += 2 * i)
                {
                    int right = j + 2 * i;
                    if (n < right) right = n;
                    merge(arr, j, j + i, right);
                    if ((itterations + 1) % 10 == 0)
                    {
                        controller.Change(arr);
                    }
                    itterations++;
                }
            }
        }
        public void SortShell(T[] arr)
        {
            int n = arr.Length;
            int itteration = 0;
            for (int k = n / 2; k > 0; k /= 2)
            {
                for (int i = k; i < n; i++)
                {
                    T t = arr[i];
                    int j = i;
                    for (j = i; j >= k; j -= k)
                    {
                        if (t.CompareTo(arr[j - k]) < 0)
                        {
                            arr[j] = arr[j - k];
                        }
                        else
                        {
                            if ((itteration + 1) % 10 == 0)
                            {
                                controller.Change(arr);
                            }
                            break;
                        }
                        if ((itteration + 1) % 10 == 0)
                        {
                            controller.Change(arr);
                        }
                        itteration++;
                    }
                    arr[j] = t;
                }
            }
        }
    }
}
