#pragma once
#include "IT.h"
#include "LT.h"
#include "Log.h"
#include <string>
#include <list>

namespace Lan {
	struct LAN {
		Log::LOG logFile;
		LT::LexTable lexTable;
		IT::IdTable idTable;
		std::string CurrentWord;
		int currentLine = 1;
		int currentPosition = 1;
		struct ListId
		{
			IT::Entry id;
			IT::Entry* pNext = nullptr;
		};
		enum Type {String,Integer} TypeIsStringOrInteger;

		enum State {Token, StringLiteral} WordStates;

		enum PossibleNextWords {Any,Function,Main,FunctionIdentifier,LeftBracketAfterFunction,RightBracketAfterFunction,ParamTypeOrRightBracket,ParamType,ParamName,ParamCommaOrRightBracket,BlockBeginsOrSemicolon,
			Semicolon,InBlockAny,InBlockDeclare,InBlockType,InBlockIdentifierOrFunction,InBlockIdentifierOrLiteral,InBlockLiteral,InBlockSemicolon,InBlockExpressionOrSemicolon,InBlockAssignment,InBlockExpression,
			InBlockReturn,InBlockPrint,	EndBlockSemicolon,} ExpectedWord; 
		
		void Inspect();
		void Confirm();
		void SetType();
		bool isIntegerOrString();
		bool isPredefinedWord();
		bool isStringLiteral();
		bool isDigitLiteral();
		bool isIdentifierName();
		bool isLeftBracket();
		bool isRightBracket();
		bool isLeftBrace();
		bool isRightBrace();
		bool isSemicolon();
		bool isComma();
		bool isAssignment();
		bool iPlusMinusSlashStar();
		bool isFunctionExpression();
		bool isDeclareExpression();
		bool isReturnExpression();
		bool isMainExpression();
		bool isPrintExpression();
		std::string IntToString(int par);
		void InspectSymbol(const unsigned char symbol);
		void WriteLexTable();
		void WriteIDTable();
		LAN(Log::LOG log);
		~LAN();
		//LT::Entry createStructLexem(char type, int number, int numberInTableId, char vType, int priority = -1);
	void Print();

	};
	void initTypeLexem(char* text, std::string tmp, LT::LexTable* tableOfLexem, int numberOfstring, int poz, IT::IdTable* newIdTable, int* typeData, int* typeID, int* littype);
	

	void CheckLTIT(LAN tables);
};

