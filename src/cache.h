#ifndef CACHE_H
#define CACHE_H

#include <bits/stdc++.h>
#include "sparsepp/spp.h"

using namespace std;

template<typename X, typename Y>
struct Node {
    X key;
    Y value;
    Node *prev, *next;
    Node(X k, Y v): key(k), value(v), prev(NULL), next(NULL) {}
};

template<typename X, typename Y>
struct DoublyLinkedList {
  Node<X, Y> *front, *rear;
  
  bool isEmpty() {
      return rear == NULL;
  }

  DoublyLinkedList(): front(NULL), rear(NULL) {}
  
  Node<X, Y>* add_page_to_head(const X &key, const Y &value) {
      Node<X, Y> *page = new Node<X, Y>(key, value);
      if(!front && !rear) {
          front = rear = page;
      }
      else {
          page->next = front;
          front->prev = page;
          front = page;
      }
      return page;
  }

  void move_page_to_head(Node<X, Y> *page) {
      if(page==front) {
          return;
      }
      if(page == rear) {
          rear = rear->prev;
          rear->next = NULL;
      }
      else {
          page->prev->next = page->next;
          page->next->prev = page->prev;
      }

      page->next = front;
      page->prev = NULL;
      front->prev = page;
      front = page;
  }

  void remove_rear_page() {
      if(isEmpty()) {
          return;
      }
      if(front == rear) {
          delete rear;
          front = rear = NULL;
      }
      else {
          Node<X, Y> *temp = rear;
          rear = rear->prev;
          rear->next = NULL;
          delete temp;
      }
  }
  Node<X, Y>* get_rear_page() {
      return rear;
  }
};

template<typename X, typename Y>
struct LRUCache{
  int capacity, size;
  DoublyLinkedList<X, Y> *pageList;
  spp::sparse_hash_map<X, Node<X, Y>*> pageMap;

    LRUCache(int capacity = 0) {
      this->capacity = capacity;
      size = 0;
        pageList = new DoublyLinkedList<X, Y>();
        pageMap = spp::sparse_hash_map<X, Node<X, Y>*>();
    }

    Y* get(const X &key) {
        if(pageMap.find(key)==pageMap.end()) {
          return NULL;
        }
        Y* val = &(pageMap[key]->value);

        // move the page to front
        pageList->move_page_to_head(pageMap[key]);
        return val;
    }

    void set(const X &key, const Y &value) {
      if(pageMap.find(key)!=pageMap.end()) {
          // if key already present, update value and move page to head
          pageMap[key]->value = value;
          pageList->move_page_to_head(pageMap[key]);
          return;
      }

        if(size == capacity) {
          // remove rear page
          X k = pageList->get_rear_page()->key;
          pageMap.erase(k);
          pageList->remove_rear_page();
          size--;
        }

        // add new page to head to Queue
        Node<X, Y> *page = pageList->add_page_to_head(key, value);
        size++;
        pageMap[key] = page;
    }
};


#endif