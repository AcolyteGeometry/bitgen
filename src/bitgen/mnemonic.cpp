/**
 * mnemonic.cpp - Bitcoin poscript generator
 *
 * Copyright (C)2015 The bitgen developers <bitgen@openmailbox.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */



#include "mnemonic.h"


#include <list>
#include <map>

#include "hash.h"
#include "bigintutil.h"

#include "ExtendedKey.h"
#include "hmac.h"



    static const char* wordList[]= { "abandon", "ability", "able", "about", "above", "absent", "absorb", 
		"abstract", "absurd", "abuse", "access", "accident", "account", "accuse", "achieve", "acid",
		"acoustic", "acquire", "across", "act", "action", "actor", "actress", "actual", "adapt", "add",
		"addict", "address", "adjust", "admit", "adult", "advance", "advice", "aerobic", "affair",
		"afford", "afraid", "again", "age", "agent", "agree", "ahead", "aim", "air", "airport",
		"aisle", "alarm", "album", "alcohol", "alert", "alien", "all", "alley", "allow", "almost", 
		"alone", "alpha", "already", "also", "alter", "always", "amateur", "amazing", "among", 
		"amount", "amused", "analyst", "anchor", "ancient", "anger", "angle", "angry", "animal", 
		"ankle", "announce", "annual", "another", "answer", "antenna", "antique", "anxiety", "any", 
		"apart", "apology", "appear", "apple", "approve", "april", "arch", "arctic", "area", 
		"arena", "argue", "arm", "armed", "armor", "army", "around", "arrange", "arrest", "arrive", 
		"arrow", "art", "artefact", "artist", "artwork", "ask", "aspect", "assault", "asset", 
		"assist", "assume", "asthma", "athlete", "atom", "attack", "attend", "attitude", "attract", 
		"auction", "audit", "august", "aunt", "author", "auto", "autumn", "average", "avocado", 
		"avoid", "awake", "aware", "away", "awesome", "awful", "awkward", "axis", "baby", "bachelor", 
		"bacon", "badge", "bag", "balance", "balcony", "ball", "bamboo", "banana", "banner", 
		"bar", "barely", "bargain", "barrel", "base", "basic", "basket", "battle", "beach", 
		"bean", "beauty", "because", "become", "beef", "before", "begin", "behave", "behind", 
		"believe", "below", "belt", "bench", "benefit", "best", "betray", "better", "between", "beyond", 
		"bicycle", "bid", "bike", "bind", "biology", "bird", "birth", "bitter", "black", "blade", "blame", 
		"blanket", "blast", "bleak", "bless", "blind", "blood", "blossom", "blouse", "blue", "blur", 
		"blush", "board", "boat", "body", "boil", "bomb", "bone", "bonus", "book", "boost", "border", 
		"boring", "borrow", "boss", "bottom", "bounce", "box", "boy", "bracket", "brain", "brand", "brass", 
		"brave", "bread", "breeze", "brick", "bridge", "brief", "bright", "bring", "brisk", "broccoli", 
		"broken", "bronze", "broom", "brother", "brown", "brush", "bubble", "buddy", "budget", 
		"buffalo", "build", "bulb", "bulk", "bullet", "bundle", "bunker", "burden", "burger", 
		"burst", "bus", "business", "busy", "butter", "buyer", "buzz", "cabbage", "cabin", "cable", 
		"cactus", "cage", "cake", "call", "calm", "camera", "camp", "can", "canal", "cancel", 
		"candy", "cannon", "canoe", "canvas", "canyon", "capable", "capital", "captain", 
		"car", "carbon", "card", "cargo", "carpet", "carry", "cart", "case", "cash", 
		"casino", "castle", "casual", "cat", "catalog", "catch", "category", "cattle", "caught", 
		"cause", "caution", "cave", "ceiling", "celery", "cement", "census", "century", "cereal", 
		"certain", "chair", "chalk", "champion", "change", "chaos", "chapter", "charge", "chase", 
		"chat", "cheap", "check", "cheese", "chef", "cherry", "chest", "chicken", "chief", 
		"child", "chimney", "choice", "choose", "chronic", "chuckle", "chunk", "churn", "cigar", 
		"cinnamon", "circle", "citizen", "city", "civil", "claim", "clap", "clarify", "claw", 
		"clay", "clean", "clerk", "clever", "click", "client", "cliff", "climb", "clinic", 
		"clip", "clock", "clog", "close", "cloth", "cloud", "clown", "club", "clump", "cluster", 
		"clutch", "coach", "coast", "coconut", "code", "coffee", "coil", "coin", "collect", 
		"color", "column", "combine", "come", "comfort", "comic", "common", "company", 
		"concert", "conduct", "confirm", "congress", "connect", "consider", "control", 
		"convince", "cook", "cool", "copper", "copy", "coral", "core", "corn", "correct", 
		"cost", "cotton", "couch", "country", "couple", "course", "cousin", "cover", "coyote", "crack", 
		"cradle", "craft", "cram", "crane", "crash", "crater", "crawl", "crazy", "cream", "credit", 
		"creek", "crew", "cricket", "crime", "crisp", "critic", "crop", "cross", "crouch", "crowd", 
		"crucial", "cruel", "cruise", "crumble", "crunch", "crush", "cry", "crystal", "cube", 
		"culture", "cup", "cupboard", "curious", "current", "curtain", "curve", "cushion", 
		"custom", "cute", "cycle", "dad", "damage", "damp", "dance", "danger", "daring", "dash", 
		"daughter", "dawn", "day", "deal", "debate", "debris", "decade", "december", "decide", 
		"decline", "decorate", "decrease", "deer", "defense", "define", "defy", "degree", "delay", 
		"deliver", "demand", "demise", "denial", "dentist", "deny", "depart", "depend", 
		"deposit", "depth", "deputy", "derive", "describe", "desert", "design", "desk", "despair", 
		"destroy", "detail", "detect", "develop", "device", "devote", "diagram", "dial", "diamond", 
		"diary", "dice", "diesel", "diet", "differ", "digital", "dignity", "dilemma", "dinner", 
		"dinosaur", "direct", "dirt", "disagree", "discover", "disease", "dish", "dismiss", 
		"disorder", "display", "distance", "divert", "divide", "divorce", "dizzy", "doctor", 
		"document", "dog", "doll", "dolphin", "domain", "donate", "donkey", "donor", "door", "dose", 
		"double", "dove", "draft", "dragon", "drama", "drastic", "draw", "dream", "dress", 
		"drift", "drill", "drink", "drip", "drive", "drop", "drum", "dry", "duck", "dumb", 
		"dune", "during", "dust", "dutch", "duty", "dwarf", "dynamic", "eager", "eagle", "early", 
		"earn", "earth", "easily", "east", "easy", "echo", "ecology", "economy", "edge", "edit", 
		"educate", "effort", "egg", "eight", "either", "elbow", "elder", "electric", "elegant", "element", 
		"elephant", "elevator", "elite", "else", "embark", "embody", "embrace", "emerge", "emotion", 
		"employ", "empower", "empty", "enable", "enact", "end", "endless", "endorse", "enemy", "energy", 
		"enforce", "engage", "engine", "enhance", "enjoy", "enlist", "enough", "enrich", "enroll", 
		"ensure", "enter", "entire", "entry", "envelope", "episode", "equal", "equip", "era", "erase", 
		"erode", "erosion", "error", "erupt", "escape", "essay", "essence", "estate", "eternal", "ethics", 
		"evidence", "evil", "evoke", "evolve", "exact", "example", "excess", "exchange", "excite", 
		"exclude", "excuse", "execute", "exercise", "exhaust", "exhibit", "exile", "exist", "exit", 
		"exotic", "expand", "expect", "expire", "explain", "expose", "express", "extend", "extra", 
		"eye", "eyebrow", "fabric", "face", "faculty", "fade", "faint", "faith", "fall", "false", 
		"fame", "family", "famous", "fan", "fancy", "fantasy", "farm", "fashion", "fat", "fatal", 
		"father", "fatigue", "fault", "favorite", "feature", "february", "federal", "fee", 
		"feed", "feel", "female", "fence", "festival", "fetch", "fever", "few", "fiber", "fiction", 
		"field", "figure", "file", "film", "filter", "final", "find", "fine", "finger", "finish", 
		"fire", "firm", "first", "fiscal", "fish", "fit", "fitness", "fix", "flag", "flame", 
		"flash", "flat", "flavor", "flee", "flight", "flip", "float", "flock", "floor", "flower", 
		"fluid", "flush", "fly", "foam", "focus", "fog", "foil", "fold", "follow", "food", "foot", 
		"force", "forest", "forget", "fork", "fortune", "forum", "forward", "fossil", "foster", "found", 
		"fox", "fragile", "frame", "frequent", "fresh", "friend", "fringe", "frog", "front", "frost", 
		"frown", "frozen", "fruit", "fuel", "fun", "funny", "furnace", "fury", "future", "gadget", 
		"gain", "galaxy", "gallery", "game", "gap", "garage", "garbage", "garden", "garlic", "garment", 
		"gas", "gasp", "gate", "gather", "gauge", "gaze", "general", "genius", "genre", "gentle", "genuine", 
		"gesture", "ghost", "giant", "gift", "giggle", "ginger", "giraffe", "girl", "give", "glad", 
		"glance", "glare", "glass", "glide", "glimpse", "globe", "gloom", "glory", "glove", "glow", 
		"glue", "goat", "goddess", "gold", "good", "goose", "gorilla", "gospel", "gossip", "govern", 
		"gown", "grab", "grace", "grain", "grant", "grape", "grass", "gravity", "great", "green", 
		"grid", "grief", "grit", "grocery", "group", "grow", "grunt", "guard", "guess", "guide", 
		"guilt", "guitar", "gun", "gym", "habit", "hair", "half", "hammer", "hamster", "hand", "happy", 
		"harbor", "hard", "harsh", "harvest", "hat", "have", "hawk", "hazard", "head", "health", "heart", 
		"heavy", "hedgehog", "height", "hello", "helmet", "help", "hen", "hero", "hidden", "high", 
		"hill", "hint", "hip", "hire", "history", "hobby", "hockey", "hold", "hole", "holiday", "hollow", 
		"home", "honey", "hood", "hope", "horn", "horror", "horse", "hospital", "host", "hotel", "hour", 
		"hover", "hub", "huge", "human", "humble", "humor", "hundred", "hungry", "hunt", "hurdle", "hurry", 
		"hurt", "husband", "hybrid", "ice", "icon", "idea", "identify", "idle", "ignore", "ill", "illegal", 
		"illness", "image", "imitate", "immense", "immune", "impact", "impose", "improve", "impulse", 
		"inch", "include", "income", "increase", "index", "indicate", "indoor", "industry", "infant", 
		"inflict", "inform", "inhale", "inherit", "initial", "inject", "injury", "inmate", "inner", 
		"innocent", "input", "inquiry", "insane", "insect", "inside", "inspire", "install", "intact", 
		"interest", "into", "invest", "invite", "involve", "iron", "island", "isolate", "issue", "item", 
		"ivory", "jacket", "jaguar", "jar", "jazz", "jealous", "jeans", "jelly", "jewel", "job", "join", 
		"joke", "journey", "joy", "judge", "juice", "jump", "jungle", "junior", "junk", "just", "kangaroo", 
		"keen", "keep", "ketchup", "key", "kick", "kid", "kidney", "kind", "kingdom", "kiss", "kit", "kitchen", 
		"kite", "kitten", "kiwi", "knee", "knife", "knock", "know", "lab", "label", "labor", "ladder", 
		"lady", "lake", "lamp", "language", "laptop", "large", "later", "latin", "laugh", "laundry", "lava", 
		"law", "lawn", "lawsuit", "layer", "lazy", "leader", "leaf", "learn", "leave", "lecture", "left", 
		"leg", "legal", "legend", "leisure", "lemon", "lend", "length", "lens", "leopard", "lesson", 
		"letter", "level", "liar", "liberty", "library", "license", "life", "lift", "light", "like", 
		"limb", "limit", "link", "lion", "liquid", "list", "little", "live", "lizard", "load", "loan", 
		"lobster", "local", "lock", "logic", "lonely", "long", "loop", "lottery", "loud", "lounge", "love", 
		"loyal", "lucky", "luggage", "lumber", "lunar", "lunch", "luxury", "lyrics", "machine", "mad", 
		"magic", "magnet", "maid", "mail", "main", "major", "make", "mammal", "man", "manage", "mandate", 
		"mango", "mansion", "manual", "maple", "marble", "march", "margin", "marine", "market", "marriage", 
		"mask", "mass", "master", "match", "material", "math", "matrix", "matter", "maximum", "maze", 
		"meadow", "mean", "measure", "meat", "mechanic", "medal", "media", "melody", "melt", "member", 
		"memory", "mention", "menu", "mercy", "merge", "merit", "merry", "mesh", "message", "metal", 
		"method", "middle", "midnight", "milk", "million", "mimic", "mind", "minimum", "minor", "minute", 
		"miracle", "mirror", "misery", "miss", "mistake", "mix", "mixed", "mixture", "mobile", "model", 
		"modify", "mom", "moment", "monitor", "monkey", "monster", "month", "moon", "moral", "more", 
		"morning", "mosquito", "mother", "motion", "motor", "mountain", "mouse", "move", "movie", "much", 
		"muffin", "mule", "multiply", "muscle", "museum", "mushroom", "music", "must", "mutual", 
		"myself", "mystery", "myth", "naive", "name", "napkin", "narrow", "nasty", "nation", "nature", 
		"near", "neck", "need", "negative", "neglect", "neither", "nephew", "nerve", "nest", "net", 
		"network", "neutral", "never", "news", "next", "nice", "night", "noble", "noise", "nominee", 
		"noodle", "normal", "north", "nose", "notable", "note", "nothing", "notice", "novel", "now", 
		"nuclear", "number", "nurse", "nut", "oak", "obey", "object", "oblige", "obscure", "observe", 
		"obtain", "obvious", "occur", "ocean", "october", "odor", "off", "offer", "office", "often", 
		"oil", "okay", "old", "olive", "olympic", "omit", "once", "one", "onion", "online", "only", 
		"open", "opera", "opinion", "oppose", "option", "orange", "orbit", "orchard", "order", "ordinary", 
		"organ", "orient", "original", "orphan", "ostrich", "other", "outdoor", "outer", "output", 
		"outside", "oval", "oven", "over", "own", "owner", "oxygen", "oyster", "ozone", "pact", "paddle", 
		"page", "pair", "palace", "palm", "panda", "panel", "panic", "panther", "paper", "parade", 
		"parent", "park", "parrot", "party", "pass", "patch", "path", "patient", "patrol", "pattern", 
		"pause", "pave", "payment", "peace", "peanut", "pear", "peasant", "pelican", "pen", "penalty", 
		"pencil", "people", "pepper", "perfect", "permit", "person", "pet", "phone", "photo", "phrase", 
		"physical", "piano", "picnic", "picture", "piece", "pig", "pigeon", "pill", "pilot", "pink", 
		"pioneer", "pipe", "pistol", "pitch", "pizza", "place", "planet", "plastic", "plate", "play", 
		"please", "pledge", "pluck", "plug", "plunge", "poem", "poet", "point", "polar", "pole", "police", 
		"pond", "pony", "pool", "popular", "portion", "position", "possible", "post", "potato", "pottery", 
		"poverty", "powder", "power", "practice", "praise", "predict", "prefer", "prepare", "present", 
		"pretty", "prevent", "price", "pride", "primary", "print", "priority", "prison", "private", 
		"prize", "problem", "process", "produce", "profit", "program", "project", "promote", "proof", 
		"property", "prosper", "protect", "proud", "provide", "public", "pudding", "pull", "pulp", 
		"pulse", "pumpkin", "punch", "pupil", "puppy", "purchase", "purity", "purpose", "purse", 
		"push", "put", "puzzle", "pyramid", "quality", "quantum", "quarter", "question", "quick", 
		"quit", "quiz", "quote", "rabbit", "raccoon", "race", "rack", "radar", "radio", "rail", 
		"rain", "raise", "rally", "ramp", "ranch", "random", "range", "rapid", "rare", "rate", "rather", 
		"raven", "raw", "razor", "ready", "real", "reason", "rebel", "rebuild", "recall", "receive", 
		"recipe", "record", "recycle", "reduce", "reflect", "reform", "refuse", "region", "regret", 
		"regular", "reject", "relax", "release", "relief", "rely", "remain", "remember", "remind", 
		"remove", "render", "renew", "rent", "reopen", "repair", "repeat", "replace", "report", 
		"require", "rescue", "resemble", "resist", "resource", "response", "result", "retire", 
		"retreat", "return", "reunion", "reveal", "review", "reward", "rhythm", "rib", "ribbon", 
		"rice", "rich", "ride", "ridge", "rifle", "right", "rigid", "ring", "riot", "ripple", 
		"risk", "ritual", "rival", "river", "road", "roast", "robot", "robust", "rocket", "romance", 
		"roof", "rookie", "room", "rose", "rotate", "rough", "round", "route", "royal", "rubber", 
		"rude", "rug", "rule", "run", "runway", "rural", "sad", "saddle", "sadness", "safe", "sail", 
		"salad", "salmon", "salon", "salt", "salute", "same", "sample", "sand", "satisfy", "satoshi", 
		"sauce", "sausage", "save", "say", "scale", "scan", "scare", "scatter", "scene", "scheme", 
		"school", "science", "scissors", "scorpion", "scout", "scrap", "screen", "script", "scrub", 
		"sea", "search", "season", "seat", "second", "secret", "section", "security", "seed", "seek", 
		"segment", "select", "sell", "seminar", "senior", "sense", "sentence", "series", "service", 
		"session", "settle", "setup", "seven", "shadow", "shaft", "shallow", "share", "shed", "shell", 
		"sheriff", "shield", "shift", "shine", "ship", "shiver", "shock", "shoe", "shoot", "shop", 
		"short", "shoulder", "shove", "shrimp", "shrug", "shuffle", "shy", "sibling", "sick", "side", 
		"siege", "sight", "sign", "silent", "silk", "silly", "silver", "similar", "simple", "since", 
		"sing", "siren", "sister", "situate", "six", "size", "skate", "sketch", "ski", "skill", 
		"skin", "skirt", "skull", "slab", "slam", "sleep", "slender", "slice", "slide", "slight", 
		"slim", "slogan", "slot", "slow", "slush", "small", "smart", "smile", "smoke", "smooth", 
		"snack", "snake", "snap", "sniff", "snow", "soap", "soccer", "social", "sock", "soda", 
		"soft", "solar", "soldier", "solid", "solution", "solve", "someone", "song", "soon", "sorry", 
		"sort", "soul", "sound", "soup", "source", "south", "space", "spare", "spatial", "spawn", 
		"speak", "special", "speed", "spell", "spend", "sphere", "spice", "spider", "spike", "spin", 
		"spirit", "split", "spoil", "sponsor", "spoon", "sport", "spot", "spray", "spread", "spring", 
		"spy", "square", "squeeze", "squirrel", "stable", "stadium", "staff", "stage", "stairs", 
		"stamp", "stand", "start", "state", "stay", "steak", "steel", "stem", "step", "stereo", 
		"stick", "still", "sting", "stock", "stomach", "stone", "stool", "story", "stove", "strategy", 
		"street", "strike", "strong", "struggle", "student", "stuff", "stumble", "style", "subject", 
		"submit", "subway", "success", "such", "sudden", "suffer", "sugar", "suggest", "suit", "summer", 
		"sun", "sunny", "sunset", "super", "supply", "supreme", "sure", "surface", "surge", "surprise", 
		"surround", "survey", "suspect", "sustain", "swallow", "swamp", "swap", "swarm", "swear", 
		"sweet", "swift", "swim", "swing", "switch", "sword", "symbol", "symptom", "syrup", "system", 
		"table", "tackle", "tag", "tail", "talent", "talk", "tank", "tape", "target", "task", "taste", 
		"tattoo", "taxi", "teach", "team", "tell", "ten", "tenant", "tennis", "tent", "term", "test", 
		"text", "thank", "that", "theme", "then", "theory", "there", "they", "thing", "this", "thought", 
		"three", "thrive", "throw", "thumb", "thunder", "ticket", "tide", "tiger", "tilt", "timber", "time", 
		"tiny", "tip", "tired", "tissue", "title", "toast", "tobacco", "today", "toddler", "toe", 
		"together", "toilet", "token", "tomato", "tomorrow", "tone", "tongue", "tonight", "tool", 
		"tooth", "top", "topic", "topple", "torch", "tornado", "tortoise", "toss", "total", "tourist", 
		"toward", "tower", "town", "toy", "track", "trade", "traffic", "tragic", "train", "transfer", 
		"trap", "trash", "travel", "tray", "treat", "tree", "trend", "trial", "tribe", "trick", "trigger", 
		"trim", "trip", "trophy", "trouble", "truck", "true", "truly", "trumpet", "trust", "truth", 
		"try", "tube", "tuition", "tumble", "tuna", "tunnel", "turkey", "turn", "turtle", "twelve", 
		"twenty", "twice", "twin", "twist", "two", "type", "typical", "ugly", "umbrella", "unable", 
		"unaware", "uncle", "uncover", "under", "undo", "unfair", "unfold", "unhappy", "uniform", 
		"unique", "unit", "universe", "unknown", "unlock", "until", "unusual", "unveil", "update", 
		"upgrade", "uphold", "upon", "upper", "upset", "urban", "urge", "usage", "use", "used", "useful", 
		"useless", "usual", "utility", "vacant", "vacuum", "vague", "valid", "valley", "valve", "van", 
		"vanish", "vapor", "various", "vast", "vault", "vehicle", "velvet", "vendor", "venture", "venue", 
		"verb", "verify", "version", "very", "vessel", "veteran", "viable", "vibrant", "vicious", 
		"victory", "video", "view", "village", "vintage", "violin", "virtual", "virus", "visa", "visit", 
		"visual", "vital", "vivid", "vocal", "voice", "void", "volcano", "volume", "vote", "voyage", 
		"wage", "wagon", "wait", "walk", "wall", "walnut", "want", "warfare", "warm", "warrior", 
		"wash", "wasp", "waste", "water", "wave", "way", "wealth", "weapon", "wear", "weasel", "weather", 
		"web", "wedding", "weekend", "weird", "welcome", "west", "wet", "whale", "what", "wheat", 
		"wheel", "when", "where", "whip", "whisper", "wide", "width", "wife", "wild", "will", "win", 
		"window", "wine", "wing", "wink", "winner", "winter", "wire", "wisdom", "wise", "wish", 
		"witness", "wolf", "woman", "wonder", "wood", "wool", "word", "work", "world", "worry", 
		"worth", "wrap", "wreck", "wrestle", "wrist", "write", "wrong", "yard", "year", "yellow", 
		"you", "young", "youth", "zebra", "zero", "zone", "zoo" };




Mnemonic::Mnemonic(const std::string& inMnemonic) : 
	MnemonicBase(verifyMnemonic(inMnemonic))
{
	verifyMnemonic(inMnemonic);
	
	//Todo: Needed?
	if(sizeof(wordList)/sizeof(char*) != 2048)
	{
		std::cout << "Size: " << sizeof(wordList)/sizeof(char*) << std::endl;
		throw std::string("Internal error, need exactly 2048 words");
	}	
}




Mnemonic::Mnemonic(const BigInt<256>& myPrivkey, const int numWords) : 
	MnemonicBase(generateVariableSize(myPrivkey, numWords).toStr())
{
}





//This method does NOT verify that the passphase is correct, 
//and that the words are present in the list of 2048 words
//Todo: class Mnemonic
ExtendedPrivateKey Mnemonic::getExtendedPrivFromMnemonic(const std::string& passphrase) const
{
	const std::string mnemonicStr = toStr();
	
	const std::string salt = "mnemonic" + passphrase;
	const std::string privKey = PbKdb2HmacSha512(mnemonicStr, salt, 2048, 64);
	
	//std::cout << "Private data (without checksum):     " << hexString(privKey) << std::endl;
	
	const ExtendedPrivateKey extendedPrivate = calculateMasterKey(privKey);
	return extendedPrivate;
}



std::string Mnemonic::verifyMnemonic(const std::string& mnemonicRaw)
{
	const std::string mnemonicStr = checkWordsInWordlist(mnemonicRaw);
	//std::cout << "Normalized mnemonic: \"" << mnemonicStr << "\"" << std::endl;
	
	verifyChecksum(mnemonicStr);
		
	return mnemonicStr;
}









//This method returns the number of words in the mnemonic,
//but will throw an exeption if any word is not in the word list
int Mnemonic::checkWords() const
{
	std::string dummy;
	std::list<std::string> words;
	if(!checkWordsInWordlist(mnemonic, dummy, words))
	{
		throw std::string("Mnemonic word error");
	}
	
	return words.size();
}




std::string Mnemonic::checkWordsInWordlist(const std::string& mnemonic)
{
	//std::cout << "Will normalize: " << mnemonic << std::endl;
	std::string ret;
	
	std::list<std::string> words;
	if(!Mnemonic::checkWordsInWordlist(mnemonic, ret, words))
	{
		throw std::string("Mnemonic error");
	}

	return ret;
}



//This method verifies that all mnemonic words are in the word list
bool Mnemonic::checkWordsInWordlist(std::string str, std::string& ret, std::list<std::string>& words, const int expectedNumWords)
{	
	//std::cout << "Checking str: \"" << str << "\"" << std::endl;
	//std::string ret;
	ret = "";
	words.clear();
	
	std::map<std::string, int> wordMap;
	
	for(int i = 0 ; i < (sizeof(wordList) / sizeof(char*)) ; i++)
	{
		wordMap.insert(std::pair<std::string, int>(wordList[i], i));
	}
	
	str += ' ';
	
	//std::list<std::string> words;
	
	//Split into words
	int lastPos(0);
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c == ' ')
		{
			const std::string word = str.substr(lastPos, i - lastPos);
			if(word.empty())
			{
				lastPos = i + 1;				
				continue;
			}
			
			//std::cout << "Word: \"" << word << "\"" << std::endl;
			if(wordMap.find(word) == wordMap.end())
			{
				std::cout << "Word not found: \"" << word << "\"" << std::endl;
				return false;
			}

			if(word.size() >= 24)
			{
				std::cout << "Too many words" << std::endl;
				return false;
			}
			
			//ret += word;
			//ret += " ";
			words.push_back(word);
			
			lastPos = i + 1;
		}
	}
	
	
	if(expectedNumWords != 0 && words.size() < 24)
	{
		std::cout << "Not enough words, got: " << words.size() << " words but need 24" << std::endl;
		return false;
	}
	
	
	bool first(true);
	for(std::list<std::string>::const_iterator it = words.begin() ;
		it != words.end() ;
		++it)
	{
		const std::string& word = (*it);
		if(first)
		{
			first = false;
		}
		else
		{
			ret += " ";
		}
		
		ret += word;
	}
	
	return true;
}




void Mnemonic::verifyChecksum(const std::string& mnemonicStr)
{
	const BigInt<264> givenChecksum = getChecksum(mnemonicStr);
	
	const BigInt<264> computedChecksum = computeChecksum(mnemonicStr);	//Compute the should-be checksum

	if(givenChecksum != computedChecksum)
	{
		std::cout << "Given checksum: " << givenChecksum << std::endl;
		std::cout << "Computed checksum: " << computedChecksum << std::endl;
		
		throw std::string("Mnemonic checksum error");
	}
}




BigInt<264> Mnemonic::computeChecksum(const std::string& mnemonicStr)
{
	BigInt<264> convNum = mnemonicToBigInt(mnemonicStr);


	const int numChecksumBits = getNumberChecksumBits(mnemonicStr);

	
	convNum >>= numChecksumBits; 	//Remove the checksum bits
		
	const BigInt<256> checksumBase(convNum);	
	
	const int numBytes = numChecksumBits * 4;
	
	const std::string binValStr = checksumBase.getBinaryLong();
	
	const int valStartPos = (binValStr.size() - numBytes);
	
	const std::string binVal = binValStr.substr(valStartPos);

	
	const std::string hash = sha256(binVal);
	//Take the first 8 bits of the hash
	const unsigned char first = hash[0];
		
	const BigInt<264> computedHashLong(first);
	
	const int numShift = (8 - numChecksumBits);
	const BigInt<264> computedHash(computedHashLong >> numShift);
	return computedHash;
}



int Mnemonic::getNumberTextWords(const std::string& mnemonicStr)
{		
	std::string dummy;
	std::list<std::string> words;
	if(!checkWordsInWordlist(mnemonicStr, dummy, words))
	{
		throw std::string("Mnemonic word error");
	}
	
	
	const int numTextWords = words.size();
	
	if(numTextWords != 3 && numTextWords != 6 && numTextWords != 9 && numTextWords != 12 && numTextWords != 15 && 
		numTextWords != 18 && numTextWords != 21 && numTextWords != 24)
	{
		std::cout << "Num words must be between 3 and 24, and must be a multiple of 3. " << std::endl;
		std::cout << "Got: " << numTextWords << " words" << std::endl;
		throw std::string("Incorrenct number of text words");
	}
	
	
	return numTextWords;
}




int Mnemonic::getNumberChecksumBits(const std::string& mnemonicStr)
{
	const int numTextWords = getNumberTextWords(mnemonicStr);		
	const int numChecksumBits = numTextWords / 3;
	return numChecksumBits;
}



//This method returns the checksum as given by the checksum bits
BigInt<264> Mnemonic::getChecksum(const std::string& mnemonicStr)
{
	BigInt<264> convNum = mnemonicToBigInt(mnemonicStr);
		
	const int numChecksumBits = getNumberChecksumBits(mnemonicStr);
		
	//The last 8 bits are the checksum
	const BigInt<264> checksum = convNum.returnMaskHighBits(numChecksumBits);
	return checksum;
}







//(3 * 11) - 1 = 32 bits	1 words
//(6 * 11) - 2 = 64 bits	2 words
//(9 * 11) - 3 = 96 bits	3 words
//(12 * 11) - 4 = 128 bits	4 words
//(15 * 11) - 5 = 160 bits	5 words
//(18 * 11) - 6 = 192 bits	6 words
//(21 * 11) - 7 = 224 bits	7 words
//(24 * 11) - 8 = 256 bits	8 words

//Returns the number respresentation of the mnemonic, max 264 bits
std::string Mnemonic::getAsNumber() const
{
	verifyChecksum(mnemonic);	
		
	const BigInt<264> convNum = mnemonicToBigInt(mnemonic);

	const int numChecksumBits = getNumberChecksumBits(mnemonic);
	
	const int numBytes = numChecksumBits * 4;
			
	return convNum.getLastBytes(numBytes);
}





BigInt<256> Mnemonic::toPrivKey() const
{
	verifyChecksum(mnemonic);	
	
	const BigInt<264> convNum = mnemonicToBigInt(mnemonic);

	const int numChecksumBits = getNumberChecksumBits(mnemonic);
					
	const BigInt<256> convNumInt(convNum >> numChecksumBits);	
	
	return convNumInt;
}









unsigned char getMneChecksum(const BigInt<256>& privkey, const int numBytes)
{
	//std::cout << "PRIV DATA TRUNCATED: " << privkey << std::endl;
	
	const std::string binValFull = privkey.getBinaryLong();
	
	const int valStart = 32 - numBytes;
	
	const std::string binVal = binValFull.substr(valStart);
	
	//std::cout << "PRIV DATA TRUNC2   : " << hexString(binVal) << std::endl;
	
	const std::string hash = sha256(binVal);
	//std::cout << "PRIV DATA HASH     : " << hexString(hash) << std::endl;
	
	const unsigned char first = hash[0];
	return first;
}




//(3 * 11) - 1 = 32 bits	1 words
//(6 * 11) - 2 = 64 bits	2 words
//(9 * 11) - 3 = 96 bits	3 words
//(12 * 11) - 4 = 128 bits	4 words
//(15 * 11) - 5 = 160 bits	5 words
//(18 * 11) - 6 = 192 bits	6 words
//(21 * 11) - 7 = 224 bits	7 words
//(24 * 11) - 8 = 256 bits	8 words

Mnemonic Mnemonic::generateVariableSize(const BigInt<256>& privkeyRaw, const int numWords)
{
	if(numWords % 3 != 0)
	{
		throw std::string("Num words must be a multiple of 3");	
	}
	
	if(numWords > 24)
	{
		throw std::string("Max 24 words permitted");			
	}
	
	const int num32bitItems = numWords / 3;	
	const int numBits = 32 * num32bitItems;
	const int numBytes = 4 * num32bitItems;
	const int numShift = (8 - num32bitItems);
	const int numChecksumBits = num32bitItems;
	
	//std::cout << "GOT PRIV DATA      : " << privkeyRaw << std::endl;
	const BigInt<256> privkey = privkeyRaw & numBits;
	//if(privkey != privkeyRaw)
	//{
	//	std::cout << "WARNING: PRIVKEY TRUNCATED" << std::endl;
	//	std::cout << "Orig: " << privkeyRaw << std::endl;
	//	std::cout << "Truc: " << privkey << std::endl;
	//}
	
	const BigInt<264> privKeyLong(privkey);		//Widen to 264 bits
	//std::cout << "PRIV KEY           : " << privKeyLong << std::endl;


	const unsigned char checksumChar = getMneChecksum(privkey, numBytes);
	const BigInt<264> checksum8Bit(checksumChar);
	const BigInt<264> checkSumBig(checksum8Bit >> numShift);
		

	const BigInt<264> privKeyShifted(privKeyLong << numChecksumBits);
	//std::cout << "PRIV KEY SHIFTED   : " << privKeyShifted << std::endl;

	const BigInt<264> total(privKeyShifted + checkSumBig);
	//std::cout << "PRIV TOTAL         : " << total << std::endl;

	//std::cout << "Total with checksum:" << total << std::endl;


	const std::string mnemonicStr = bigIntToMnemonic(total, numWords);
	
	const Mnemonic mnemonic(mnemonicStr);
	return mnemonic;	
}



//  11 bits per word: 
//11 * 23 = 253
//11 * 24 = 264
//11 * 25 = 275
//std::string

//Todo: Remove, use above method instead
Mnemonic Mnemonic::generate(const BigInt<256>& privkey)
{
	return generateVariableSize(privkey, 24);
	
/*	
	std::string ret;

	const std::string binVal = privkey.getBinaryLong();
	const std::string hash = sha256(binVal);	
	const unsigned char first = hash[0];	//Take the first 8 bits of the hash

	const BigInt<264> hashStart(first);	
	const BigInt<264> privKeyLong(privkey);

	BigInt<264> total((privKeyLong << 8) + hashStart);

	if(sizeof(wordList)/sizeof(char*) != 2048)
	{
		std::cout << "Size: " << sizeof(wordList)/sizeof(char*) << std::endl;
		throw std::string("Internal error, need exactly 2048 words");
	}


	for(int i = 0 ; i < 24 ; i++)
	{		
		const BigInt<264> maskVal = total.returnMaskHighBits(11);
		const int val = maskVal.intVal();
		//std::cout << val << " ";
		//std::cout << wordList[val] << " ";
		
		if(i != 0)
		{
			ret = " " + ret;
		}
		
		ret = wordList[val] + ret;
		total >>= 11;
	}

	//std::cout << std::endl;


	const Mnemonic mnmonic(ret, false);
	return mnmonic;
	*/
}






