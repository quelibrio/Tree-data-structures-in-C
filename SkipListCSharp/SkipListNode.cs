using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataStructures
{
    internal class SkipListNode : IDisposable
    {
        public SkipListNode[] forward;
        public DictionaryEntry entry;

        public SkipListNode(int level)
        {
            forward = new SkipListNode[level];
        }

        public SkipListNode(int level, object key, object val)
        {
            forward = new SkipListNode[level];
            entry.Key = key;
            entry.Value = val;
        }

        public void Dispose()
        {
            for (int i = 0; i < forward.Length; i++)
            {
                forward[i] = null;
            }
        }
    }
}

