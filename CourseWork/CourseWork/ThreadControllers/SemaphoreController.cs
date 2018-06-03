using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CourseWork.ThreadControllers
{
    class SemaphoreController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        Semaphore waitChangeHandler = new Semaphore(1,1);
        bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        T[] m1;
        public SemaphoreController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>();
        }

        public void RunFind(object obj)
        {
            T key = (T)obj;
            Thread.Sleep(1);
            while (firstSorting || secondSorting)
            {
                waitChangeHandler.WaitOne();
                try
                {
                    T foundKey = finder.Find(key, this.m1);
                }
                catch (KeyNotFoundException e)
                {
                }
                waitChangeHandler.Release();
            }
        }
        public void Change(T[] arr)
        {
            
            waitChangeHandler.WaitOne();
            m1 = new T[arr.Length];
            Array.Copy(arr, m1, arr.Length);
            waitChangeHandler.Release();
        }

        public void RunSortMerge(object obj)
        {
            T[] arr = (T[])obj;
            sorter.SortMerge(arr);
            firstSorting = false;
        }

        public void RunSortShell(object obj)
        {
            T[] arr = (T[])obj;
            sorter.SortShell(arr);
            secondSorting = false;
        }
    }
}
