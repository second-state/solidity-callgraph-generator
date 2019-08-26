/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <libsolidity/ast/ASTVisitor.h>
#include <libsolidity/interface/GasEstimator.h>
#include <map>
#include <ostream>

namespace dev
{
namespace solidity
{

class ASTCallgraph: public ASTConstVisitor
{
public:
	ASTCallgraph(): m_activeContractId(-1), m_activeScopeId(-1){}
	/// Output the string representation of the callgraph to _stream.
	void print(ASTNode const& _ast, std::ostream& _stream);

	bool visit(ContractDefinition const& _node) override;
	bool visit(InheritanceSpecifier const& _node) override;
	bool visit(FunctionDefinition const& _node) override;
	bool visit(ModifierDefinition const& _node) override;
	bool visit(EventDefinition const& _node) override;
	bool visit(NewExpression const& _node) override;
	bool visit(MemberAccess const& _node) override;
	bool visit(Identifier const& _node) override;

	void endVisit(SourceUnit const& _node) override;
	void endVisit(ContractDefinition const& _node) override;
	void endVisit(FunctionDefinition const& _node) override;
	void endVisit(ModifierDefinition const& _node) override;
	void endVisit(EventDefinition const& _node) override;

private:
	void writeLine(std::string const& _line);

	bool m_isInheritenceOrFunctionCall;
	int m_activeContractId;
	std::string m_activeContractName;
	int m_activeScopeId;
	std::map<int, std::string> m_idToSignatureMap;
	std::vector<std::pair<int, int>> m_callGraphEdges;
	std::ostream* m_ostream = nullptr;
};

}
}
