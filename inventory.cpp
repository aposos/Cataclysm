#include <sstream>
#include "inventory.h"
#include "game.h"

item& inventory::operator[] (int i)
{
 if (i < 0 || i > items.size()) {
  debugmsg("Attempted to access item %d in an inventory (size %d)",
           i, items.size());
  return nullitem;
 }

 return items[i][0];
}

std::vector<item>& inventory::stack_at(int i)
{
 if (i < 0 || i > items.size()) {
  debugmsg("Attempted to access stack %d in an inventory (size %d)",
           i, items.size());
  return nullstack;
 }
 return items[i];
}

int inventory::size() const
{
 return items.size();
}

inventory& inventory::operator= (inventory &rhs)
{
 if (this == &rhs)
  return *this; // No self-assignment

 items.clear();
 for (int i = 0; i < rhs.size(); i++)
  items.push_back(rhs.stack_at(i));
 return *this;
}

inventory& inventory::operator+= (inventory &rhs)
{
 for (int i = 0; i < rhs.size(); i++)
  add_stack(rhs.stack_at(i));
 return *this;
}

inventory& inventory::operator+= (const std::vector<item> &rhs)
{
 for (int i = 0; i < rhs.size(); i++)
  add_item(rhs[i]);
 return *this;
}

inventory& inventory::operator+= (const item &rhs)
{
 add_item(rhs);
 return *this;
}

inventory inventory::operator+ (inventory &rhs)
{
 return inventory(*this) += rhs;
}

inventory inventory::operator+ (const std::vector<item> &rhs)
{
 return inventory(*this) += rhs;
}

inventory inventory::operator+ (const item &rhs)
{
 return inventory(*this) += rhs;
}

void inventory::clear()
{
 items.clear();
}

void inventory::add_stack(std::vector<item> newits)
{
 for (int i = 0; i < newits.size(); i++)
  add_item(newits[i]);
}

void inventory::push_back(std::vector<item> newits)
{
 add_stack(newits);
}
 
void inventory::add_item(item newit)
{
 for (int i = 0; i < items.size(); i++) {
  if (items[i][0].stacks_with(newit)) {
   newit.invlet = items[i][0].invlet;
   items[i].push_back(newit);
   return;
  }
 }
 if (!newit.invlet_is_okay() || index_by_letter(newit.invlet) != -1) {
  int ch;
  for (ch = 'a'; ch <= 'z' && ch > 0; ch++) {
   if (index_by_letter(ch) == -1) {
    newit.invlet = ch;
    ch = -1;
   }
  }
  if (ch > 0) {
   for (ch = 'A'; ch <= 'Z' && ch > 0; ch++) {
    if (index_by_letter(ch) == -1) {
     newit.invlet = ch;
     ch = -1;
    }
   }
  }
  if (ch > 0)
   newit.invlet = '`';
 }
 std::vector<item> newstack;
 newstack.push_back(newit);
 items.push_back(newstack);
}

void inventory::push_back(item newit)
{
 add_item(newit);
}

void inventory::restack(game *g)
{
 inventory tmp;
 for (int i = 0; i < size(); i++) {
  for (int j = 0; j < items[i].size(); j++)
   tmp.add_item(items[i][j]);
 }
 clear();
 if (g != NULL) {
  for (int i = 0; i < tmp.size(); i++) {
   if (g->u.has_weapon_or_armor(tmp[i].invlet)) {
    int ch;
    for (ch = 'a'; ch <= 'z' && ch > 0; ch++) {
     if (!g->u.has_weapon_or_armor(ch) && tmp.index_by_letter(ch == -1)) {
      for (int j = 0; j < tmp.stack_at(i).size(); j++)
       tmp.stack_at(i)[j].invlet = ch;
      ch = -1;
     }
    }
    if (ch > 0) {
     for (ch = 'A'; ch <= 'Z' && ch > 0; ch++) {
      if (!g->u.has_weapon_or_armor(ch) && tmp.index_by_letter(ch == -1)) {
       for (int j = 0; j < tmp.stack_at(i).size(); j++)
        tmp.stack_at(i)[j].invlet = ch;
       ch = -1;
      }
     }
    }
    if (ch > 0) {
     for (int j = 0; j < tmp.stack_at(i).size(); j++)
      tmp.stack_at(i)[j].invlet = '`';
    }
   }
  }
 }
 for (int i = 0; i < tmp.size(); i++)
  items.push_back(tmp.stack_at(i));
}

void inventory::form_from_map(game *g, point origin, int range)
{
 items.clear();
 for (int x = origin.x - range; x <= origin.x + range; x++) {
  for (int y = origin.y - range; y <= origin.y + range; y++) {
   for (int i = 0; i < g->m.i_at(x, y).size(); i++)
    add_item(g->m.i_at(x, y)[i]);
  }
 }
}

std::vector<item> inventory::remove_stack(int index)
{
 if (index < 0 || index >= items.size()) {
  debugmsg("Tried to remove_stack(%d) from an inventory (size %d)",
           index, items.size());
  std::vector<item> nullvector;
  return nullvector;
 }
 std::vector<item> ret = stack_at(index);
 items.erase(items.begin() + index);
 return ret;
}

item inventory::remove_item(int index)
{
 if (index < 0 || index >= items.size()) {
  debugmsg("Tried to remove_item(%d) from an inventory (size %d)",
           index, items.size());
  return nullitem;
 }

 item ret = items[index][0];
 items[index].erase(items[index].begin());
 if (items[index].empty())
  items.erase(items.begin() + index);

 return ret;
}

item inventory::remove_item(int stack, int index)
{
 if (stack < 0 || stack >= items.size()) {
  debugmsg("Tried to remove_item(%d, %d) from an inventory (size %d)",
           stack, index, items.size());
  return nullitem;
 } else if (index < 0 || index >= items[stack].size()) {
  debugmsg("Tried to remove_item(%d, %d) from an inventory (stack is size %d)",
           stack, index, items[stack].size());
  return nullitem;
 }

 item ret = items[stack][index];
 items[stack].erase(items[stack].begin() + index);
 if (items[stack].empty())
  items.erase(items.begin() + stack);

 return ret;
}

item inventory::remove_item_by_letter(char ch)
{
 for (int i = 0; i < items.size(); i++) {
  if (items[i][0].invlet == ch)
   return remove_item(i);
 }

 return nullitem;
}

item& inventory::item_by_letter(char ch)
{
 for (int i = 0; i < items.size(); i++) {
  if (items[i][0].invlet == ch)
   return items[i][0];
 }
 return nullitem;
}

int inventory::index_by_letter(char ch)
{
 for (int i = 0; i < items.size(); i++) {
  if (items[i][0].invlet == ch)
   return i;
 }
 return -1;
}

int inventory::amount_of(itype_id it)
{
 int count = 0;
 for (int i = 0; i < items.size(); i++) {
  for (int j = 0; j < items[i].size(); j++) {
   if (items[i][j].type->id == it)
    count++;
   for (int k = 0; k < items[i][j].contents.size(); k++) {
    if (items[i][j].contents[k].type->id == it)
     count++;
   }
  }
 }
 return count;
}

int inventory::charges_of(itype_id it)
{
 int count = 0;
 for (int i = 0; i < items.size(); i++) {
  for (int j = 0; j < items[i].size(); j++) {
   if (items[i][j].type->id == it)
    count += items[i][j].charges;
   for (int k = 0; k < items[i][j].contents.size(); k++) {
    if (items[i][j].contents[k].type->id == it)
     count += items[i][j].contents[k].charges;
   }
  }
 }
 return count;
}

void inventory::use_amount(itype_id it, int quantity)
{
 for (int i = 0; i < items.size() && quantity > 0; i++) {
  for (int j = 0; j < items[i].size() && quantity > 0; j++) {
// First, check contents
   for (int k = 0; k < items[i][j].contents.size() && quantity > 0; k++) {
    if (items[i][j].contents[k].type->id == it) {
     quantity--;
     items[i][j].contents.erase(items[i][j].contents.begin() + k);
     k--;
    }
   }
// Now check the item itself
   if (items[i][j].type->id == it) {
    quantity--;
    items[i].erase(items[i].begin() + j);
    j--;
    if (items[i].empty()) {
     items.erase(items.begin() + i);
     i--;
     j = 0;
    }
   }
  }
 }
}

void inventory::use_charges(itype_id it, int quantity)
{
 for (int i = 0; i < items.size() && quantity > 0; i++) {
  for (int j = 0; j < items[i].size() && quantity > 0; j++) {
// First, check contents
   for (int k = 0; k < items[i][j].contents.size() && quantity > 0; k++) {
    if (items[i][j].contents[k].type->id == it) {
     if (items[i][j].contents[k].charges < quantity) {
      quantity -= items[i][j].contents[k].charges;
      if (items[i][j].contents[k].destroyed_at_zero_charges()) {
       items[i][j].contents.erase(items[i][j].contents.begin() + k);
       k--;
      } else
       items[i][j].contents[k].charges = 0;
     } else {
      items[i][j].contents[k].charges -= quantity;
      return;
     }
    }
   }
// Now check the item itself
   if (items[i][j].type->id == it) {
    if (items[i][j].charges < quantity) {
     quantity -= items[i][j].charges;
     if (items[i][j].destroyed_at_zero_charges()) {
      items[i].erase(items[i].begin() + j);
      j--;
      if (items[i].empty()) {
       items.erase(items.begin() + i);
       i--;
       j = 0;
      }
     } else
      items[i][j].charges = 0;
    } else {
     items[i][j].charges -= quantity;
     return;
    }
   }

  }
 }
}
 
bool inventory::has_amount(itype_id it, int quantity)
{
 return (amount_of(it) >= quantity);
}

bool inventory::has_charges(itype_id it, int quantity)
{
 return (charges_of(it) >= quantity);
}

bool inventory::has_item(item *it)
{
 for (int i = 0; i < items.size(); i++) {
  for (int j = 0; j < items[i].size(); j++) {
   if (it == &(items[i][j]))
    return true;
   for (int k = 0; k < items[i][j].contents.size(); k++) {
    if (it == &(items[i][j].contents[k]))
     return true;
   }
  }
 }
 return false;
}
