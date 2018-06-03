using System;
using System.Collections.Generic;
using System.Threading;

namespace CourseWork.ThreadControllers
{
    class EventController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        AutoResetEvent waitChangeHandler = new AutoResetEvent(false);
        AutoResetEvent waitFindHandler = new AutoResetEvent(true);
        bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        T[] m1;
        public EventController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>();
        }
        
        public void RunFind(object obj)
        {
            T key = (T)obj;
            while (firstSorting || secondSorting) {
                waitChangeHandler.WaitOne();
                waitChangeHandler.Reset();
                try
                {
                    T foundKey = finder.Find(key, this.m1);
                }
                catch (KeyNotFoundException e)
                {
                }
                waitFindHandler.Set();
            }
        }
        public void Change(T[] arr)
        {
            waitFindHandler.WaitOne();
            waitFindHandler.Reset();
            m1 = new T[arr.Length];
            Array.Copy(arr, m1, arr.Length);
            waitChangeHandler.Set();
        }

        public void RunSortMerge(object obj)
        {
            T[] arr = (T[]) obj;
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
