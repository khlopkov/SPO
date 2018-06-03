using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CourseWork.ThreadControllers
{
    class CriticalSectionController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        
        bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        static object changeLocker = new object();
        static object findLocker = new object();
        T[] m1;
        public CriticalSectionController()
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
                lock (findLocker)
                {
                    try
                    {
                        T foundKey = finder.Find(key, this.m1);
                        
                    }
                    catch (KeyNotFoundException e)
                    {
                        
                    }
                }
                
            }
        }
        public void Change(T[] arr)
        {
            lock (findLocker)
            {
                m1 = new T[arr.Length];
                Array.Copy(arr, m1, arr.Length);
            }
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

