//
// Created by Efecan Kasapoğlu on 22.10.2024.
//

#ifndef CS300_TH1_STACK_H
#define CS300_TH1_STACK_H

template <class Object>
class stack{
public:
    stack();
    ~stack();
    bool isEmpty() const;
    void makeEmpty();
    void push(const Object &x);
    void pop();
    Object top() const;
private:
    struct ListNode
    {
        Object element;
        ListNode *next;
        ListNode(const Object & theElement, ListNode*n=nullptr)
                :element(theElement), next(n) {}
    };
    ListNode *topOfStack;
};

//implementation part
template <class Object>
stack<Object>::stack() //default constructor
{
    topOfStack = nullptr;
}

template <class Object>//deconstructor
stack<Object>::~stack(){
    makeEmpty();
}

template <class Object>
bool stack<Object>::isEmpty() const{ //checks if it is empty
    return topOfStack==nullptr;
}


template <class Object>
void stack<Object>::makeEmpty(){ //makes empty
    while(!isEmpty()){
        pop();
    }
}

template <class Object>
void stack<Object>::push(const Object &x){//pushes the x to the stack
    topOfStack=new ListNode(x,topOfStack);
}

template <class Object>
void stack<Object>::pop(){//deletes the last element which is added to the stack
    if (isEmpty()){
        throw std::underflow_error("Stack is empty, cannot pop");
    }
    ListNode *oldTop=topOfStack;
    topOfStack=topOfStack->next;
    delete oldTop;
}

template <class Object>
Object stack<Object>::top() const{ //returns to the top of the stack
    if (isEmpty()){
        throw std::underflow_error("Stack is empty, no top element");
    }
    return topOfStack->element;
}


#endif //CS300_TH1_STACK_H
