public class ObjectList
{
	class Link {
		internal object it;
		internal Link next;
		internal Link(object o, Link x) { it=o;next=x; }
	}		
	void Add0(Link a) {
		if (head==null)
			head = last = a;
		else
			last = last.next = a;
	}
	object Get0(Link a,int x) { 
		if (a==null || x<0)  // safety
			return null;
		if (x==0)
			return a.it;
		return Get0(a.next,x-1);
	}
	bool RemoveAt0(ref Link h,ref Link t,int x) {
		if (h==null || x<0) // safety
			return false;
		if (x==0) {
			h = h.next;
			if (h==null)
				t = h;
			return true;
		}
		bool r = RemoveAt0(ref h.next, ref t, x-1);
		if (h.next == null)
				t = h;
		return r;
	}
	private Link head = null, last=null;
	private int count = 0;
	public ObjectList() {}
	public void Add(object o) { Add0(new Link(o,null)); count++; }
	public void RemoveAt(int x) {
		if (RemoveAt0(ref head, ref last, x))
			count--;
	}
	public int Count { get { return count; }}
	public object this[int ix] { get { return Get0(head,ix); } }
}
