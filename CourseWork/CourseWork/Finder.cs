using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CourseWork
{
    class Finder<T> where T : System.IComparable<T>
    {
        private ThreadController<T> controller;
        public Finder(ThreadController<T> controller)
        {
            this.controller = controller;
        }
        public T Find(T key, T[] arr)
        {
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i].CompareTo(key) == 0)
                {
                    return arr[i];
                }
            }
            throw new KeyNotFoundException("Key not Found");
        }
    }
}
