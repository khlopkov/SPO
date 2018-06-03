using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CourseWork
{
    class Finder
    {
        private ThreadController controller;
        public Finder(ThreadController controller)
        {
            this.controller = controller;
        }
        public int Find(int key, int[] arr)
        {
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i] == key)
                {
                    return arr[i];
                }
                else
                {
                    if (i == arr.Length - 1)
                        Console.WriteLine("No such key");
                }
            }
            return -1;
        }
    }
}
