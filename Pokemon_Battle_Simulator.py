import random
import math


class Pokemon:
  def __init__(self, name, level, ptype, maxhealth, currenthealth, KOed, move):
    self.name = name
    self.level = level
    self.ptype = ptype
    self.maxhp = maxhealth
    self.currenthp = currenthealth
    self.KOstatus = KOed
    self.move = move
  
  def pokemon_id(self):
    if (self.KOstatus == True):
      return("{name} is KOed!".format(name=self.name))
    else:
      return("{name}, a level {level} {type} type "
            "has {ch} out of {mh} HP.".format(level=self.level, 
            name=self.name, type=self.ptype, ch=self.currenthp, mh=self.maxhp))
      

  def health_change(self, amount):
    if (amount > 0):
      self.currenthp -= amount
      print("{name} has lost {amount} HP!"
            .format(name=self.name, amount = amount))
      if (self.currenthp < 0):
        self.KOstatus = True
        self.currenthp = 0
        print(f"{self.name} has fainted!")
      print(f"{self.name} has {self.currenthp} out of {self.maxhp} HP left!")
        
    elif (amount == 0):
      print("No damage was done to {name}'s HP! ".format(name=self.name))      
      
    else:
      print("{name} has regained {amount} HP! ".format(name=self.name, 
                                                       amount=amount))
      self.currenthp += amount
      if (self.currenthp > self.maxhp):
        self.currenthp = self.maxhp

    self.pokemon_id()      
	  

class Move:
  def __init__(self, name, criticalchance, atype, damage, accuracy, pp):
    self.name = name
    self.cc = criticalchance
    self.atype = atype
    self.damage = damage
    self.accuracy = accuracy
    self.pp = pp
    
  def attack_scan(self):
    return("This is {an}, a {t} attack. It has {a}% chance of doing {d} damage "
          "with a {c}% chance of delivering a critical. There are {p} PPs left."
          .format(an=self.name, t=self.atype, d=self.damage, c=self.cc, 
                  a=self.accuracy, p=self.pp))
    
class Battle:
  def __init__(self, Pokemon1, Pokemon2):
    self.p1 = Pokemon1
    self.p2 = Pokemon2
    
  def context(self):
    print("{p1} is now battling {p2}!\n{p1_info}\n{p2_info}"
          .format(p1=self.p1.name, p2=self.p2.name,
                  p1_info=self.p1.pokemon_id(), 
                  p2_info=self.p2.pokemon_id()))    

    
# returns 0 if both pokemon are of same type, 1 if the Pokemon1 has a   
# type advantage, 2 if Pokemon2 has a type advantage	
  def type_matchup(self):
      type1 = self.p1.ptype
      type2 = self.p2.ptype  
      if (type1 == type2): return 0
      elif (type1 == "fire"):
        if (type2 == "water"): return 2
        if (type2 == "plant"): return 1
      elif (type1 == "plant"):
        if (type2 == "water"): return 1
        if (type2 == "fire"): return 2  
      elif (type1 == "water"):
        if (type2 == "plant"): return 2
        if (type2 == "fire"): return 1 
        
# Assume Pokemon1 is the attacking pokemon, and Pokemon2 is the defending. 
# Attack power is determined by the offical pokemon damage calculation forumla
# but with nuances such as STAB and Weather effects removed.        
        
  def hp_damage(self):
    print(f"{self.p1.name} uses {self.p1.move.name}!")
    type_advantage = self.type_matchup()
    same_type = (self.p1.ptype == self.p1.move.atype)
    power = ((((2 * self.p1.level) / 5) + 2) * self.p1.move.damage) / 50
    critical = (self.p1.move.cc >= random.randint(1,101))
    # generating if move hits or not
    
    if (random.randint(1,101) <= self.p1.move.accuracy):
      if same_type:
        power *= 1.5
      if (type_advantage == 1):
        print("It's super effective.")
        power *= 2
      if (type_advantage == 2):
        print("It's not very effective.")
        power *= 0.5
      if critical:
        print("A critical hit!")
        power *= 1.5
      return math.floor(power)
    else:
      print("The attack has missed.")
      return 0
    
  def BattleMech(self):
    reverse_battle = Battle(self.p2, self.p1)
    self.context()
    while (self.p1.KOstatus == False and self.p2.KOstatus == False):
      player_decision = input("What would you like {pname} to do?\n"
                              "[a]Attack\n[b]Flee\n[c]Scan Move[d]Context\n"
                              .format(pname = p1.name))
      if (player_decision == 'a'):
        damage = self.hp_damage()
        p2.health_change(damage)
        # alternative pokemone attacks
        print("\n")
        damage = reverse_battle.hp_damage()
        p1.health_change(damage)
      if (player_decision == 'b'):
        print("You have fled the scene")
        break 
      if (player_decision == 'c'):
        print(self.p1.move.attack_scan())
        continue
      if (player_decision == 'd'):
        self.context()
        continue
      
    

class Menu:
  def __init__(self):
    pass
  
  def pokemon_pickings(self):
    play_again = True
    while play_again:
      print("The three pokemon currently in this game are: Bulbasaur, Squirtle, Charmander")
      player_pokemon = input("Which pokemon do you want?\n[b]Bulbasaur\n"
                             "[s]Squirtle\n[c]Charmander\n")
      duplicate_pokemon = dict(pokemon)   
      del duplicate_pokemon[player_pokemon]
      print("Which pokemon do you want to battle?")
      for value in duplicate_pokemon.values():
        if value == p3:
          print("[b]Bulbasaur")
        if value == p2:
          print("[s]Squirtle")
        if value == p1:
          print("[c]Charmander")
      opponent_pokemon = input("")
      game = Battle(pokemon[player_pokemon], pokemon[opponent_pokemon])
      game.BattleMech()  
      play_again = input("Would you like to play again? (y/n)\n")
      if play_again != 'y':
        print("Thanks for checking out my quick Pokemon battle simulator!")
        break


def main():
  menu = Menu()
  menu.pokemon_pickings()

# Below are variables representing pokemon, and their moves
types = ["fire", "water" "plant"]
water_atk = Move("Water Gun", 10, "water", 40, 60, 20)
plant_atk = Move("Vine Whip", 30, "plant", 30, 70, 20)
fire_atk = Move("Flamethrower", 40, "fire", 40, 50, 20)
p1 = Pokemon("Charmander", 5, "fire", 22, 21, False, fire_atk)
p2 = Pokemon("Squirtle", 4, "water", 23, 20, False, water_atk)
p3 = Pokemon("Bulbasaur", 6, "plant", 22, 22, False, plant_atk)
pokemon = {"b": p3, "s": p2, "c": p1} 
# End of variable list

if __name__ == "__main__":
  main()
