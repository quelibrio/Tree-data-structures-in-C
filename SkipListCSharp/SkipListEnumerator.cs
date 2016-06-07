using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataStructures
{
    internal class SkipListEnumerator : IDictionaryEnumerator
    {
        // The skip list to enumerate.
        private SkipList list;

        // The current node.
        private SkipListNode current;

        // The version of the skip list we are enumerating.
        private long version;

        // Keeps track of previous move result so that we can know 
        // whether or not we are at the end of the skip list.
        private bool moveResult = true;

        public SkipListEnumerator(SkipList list)
        {
            this.list = list;
            this.version = list.version;
            this.current = list.nillNode;
        }

        /// <summary>
        /// Gets both the key and the value of the current dictionary 
        /// entry.
        /// </summary>
        public DictionaryEntry Entry
        {
            get
            {
                DictionaryEntry entry;

                // Make sure the skip list hasn't been modified since the
                // enumerator was created.
                if (version != list.version)
                {
                    string msg = list.resManager.GetString("InvalidEnum");
                    throw new InvalidOperationException(msg);
                }
                // Make sure we are not before the beginning or beyond the 
                // end of the skip list.
                else if (current == list.nillNode)
                {
                    string msg = list.resManager.GetString("BadEnumAccess");
                    throw new InvalidOperationException();
                }
                // Finally, all checks have passed. Get the current entry.
                else
                {
                    entry = current.entry;
                }

                return entry;
            }
        }

        public object Key
        {
            get
            {
                object key = Entry.Key;

                return key;
            }
        }

        public object Value
        {
            get
            {
                object val = Entry.Value;

                return val;
            }
        }

        public bool MoveNext()
        {
            if (version == list.version)
            {
                if (moveResult)
                {
                    current = current.forward[0];

                    // If we are at the end of the skip list.
                    if (current == list.nillNode)
                    {
                        // Indicate that we've reached the end of the skip 
                        // list.
                        moveResult = false;
                    }
                }
            }
            // Else this version of the enumerator doesn't match that of 
            // the skip list. The skip list has been modified since the 
            // creation of the enumerator.
            else
            {
                string msg = list.resManager.GetString("InvalidEnum");
                throw new InvalidOperationException(msg);
            }

            return moveResult;
        }

        /// <summary>
        /// Sets the enumerator to its initial position, which is before 
        /// the first element in the skip list.
        /// </summary>
        public void Reset()
        {
            // Make sure the skip list hasn't been modified since the
            // enumerator was created.
            if (version == list.version)
            {
                current = list.nillNode;
                moveResult = true;
            }
            // Else this version of the enumerator doesn't match that of 
            // the skip list. The skip list has been modified since the 
            // creation of the enumerator.
            else
            {
                string msg = list.resManager.GetString("InvalidEnum");
                throw new InvalidOperationException(msg);
            }
        }

        /// <summary>
        /// Gets the current element in the skip list.
        /// </summary>
        public object Current
        {
            get
            {
                return Value;
            }
        }
    }   
}
